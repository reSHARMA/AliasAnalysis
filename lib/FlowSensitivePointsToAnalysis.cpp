#include "FlowSensitivePointsToAnalysis.h"
#include "iostream"
#include "map"
#include "set"
#include "spatial/Benchmark/PTABenchmark.h"
#include "spatial/Graph/Graph.h"
#include "spatial/InstModel/GenericInstModel/GenericInstModel.h"
#include "spatial/Token/Token.h"
#include "spatial/Token/TokenWrapper.h"
#include "spatial/Utils/CFGUtils.h"
#include "stack"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"

using namespace llvm;
using PointsToGraph = spatial::Graph<spatial::Token>;

namespace FlowSensitiveAA {

class PointsToAnalysis {
private:
  PointsToGraph GlobalPointsToGraph;
  std::map<Instruction *, PointsToGraph> PointsToIn, PointsToOut;
  spatial::TokenWrapper *TW;
  spatial::GenericInstModel *IM;
  spatial::PTABenchmarkRunner *Bench;
  std::stack<llvm::Instruction *> WorkList;
  std::map<llvm::Function *, std::set<llvm::Instruction *>> CallGraph;

public:
  PointsToAnalysis(Module &M) {
    TW = new spatial::TokenWrapper();
    IM = new spatial::GenericInstModel(TW);
    Bench = new spatial::PTABenchmarkRunner();
    initializeWorkList(M);
    handleGlobalVar(M);
  }
  void handleGlobalVar(llvm::Module &M) {
    // Handle global variables
    for (auto &G : M.getGlobalList()) {
      auto Tokens = IM->extractToken(&G);
      auto Redirections = IM->extractRedirections(&G);
      if (Tokens.size() == 2) {
        GlobalPointsToGraph.insert(Tokens[0], Tokens[1], Redirections[0],
                                   Redirections[1]);
        // Handle the case when a global variable is initialized with an
        // address
        if (llvm::GlobalVariable *Constant =
                llvm::dyn_cast<GlobalVariable>(G.getInitializer())) {
          GlobalPointsToGraph.insert(Tokens[0], TW->getToken(Constant), 2, 1);
        }
      }
    }
  }
  void initializeWorkList(llvm::Module &M) {
    for (Function &F : M.functions()) {
      for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
        if (I == inst_begin(F)) {
          WorkList.push(&*I);
        }
      }
    }
  }
  void runOnWorkList() {
    while (!WorkList.empty()) {
      Instruction *Inst = WorkList.top();
      WorkList.pop();
      PointsToGraph OldPointsToInfo = PointsToOut[Inst];
      runAnalysis(Inst);
      PointsToGraph NewPointsToInfo = PointsToOut[Inst];
      if (!(OldPointsToInfo == NewPointsToInfo)) {
        for (Instruction *I : spatial::GetSucc(Inst)) {
          WorkList.push(I);
        }
      }
    }
  }
  void handleReturnValue(llvm::Instruction *Inst) {
    PointsToOut[Inst] = PointsToIn[Inst];
    auto Tokens = IM->extractToken(Inst);
    if (CallInst *CI = dyn_cast<CallInst>(Inst)) {
      Function &Func = *CI->getCalledFunction();
      // handle return value
      if (!CI->doesNotReturn()) {
        if (ReturnInst *RI = dyn_cast<ReturnInst>(&(Func.back().back()))) {
          auto CallTokens = IM->extractToken(RI);
          if (CallTokens.size() == 1) {
            PointsToOut[&(Func.back().back())].insert(Tokens[0], CallTokens[0],
                                                      1, 1);
          }
        }
      }
      // handle change made to globals
      for (auto P : PointsToOut[&Func.back().back()]) {
        if (!P.first->sameFunc(&Func)) {
          for (auto X : P.second) {
            if (!X->sameFunc(&Func))
              PointsToOut[Inst].insert(P.first, X);
          }
        }
      }
    }
  }
  void runAnalysis(llvm::Instruction *Inst) {
    llvm::BasicBlock *ParentBB = Inst->getParent();
    llvm::Function *ParentFunc = ParentBB->getParent();
    std::vector<PointsToGraph> Predecessors;
    // Handle function arguments
    PointsToGraph ArgPointsToGraph;
    for (auto Arg = ParentFunc->arg_begin(); Arg != ParentFunc->arg_end();
         Arg++) {
      auto Tokens = IM->extractToken(Arg, ParentFunc);
      if (Tokens.size() == 2)
        ArgPointsToGraph.insert(Tokens[0], Tokens[1], 1, 0);
    }
    // Only calculate aliases for global variables and arguments at
    // the
    // start of the function
    if (&ParentBB->front() == Inst) {
      Predecessors.push_back(GlobalPointsToGraph);
      Predecessors.push_back(ArgPointsToGraph);
    }
    // Calculate control flow predecessor
    for (Instruction *I : spatial::GetPred(Inst)) {
      if (PointsToOut.find(I) != PointsToOut.end())
        Predecessors.push_back(PointsToOut[I]);
    }
    PointsToIn[Inst].merge(Predecessors);
    PointsToOut[Inst] = PointsToIn[Inst];
    // Extract alias tokens from the instruction
    auto Tokens = IM->extractToken(Inst);
    // Find the relative redirection between lhs and rhs
    // example for a = &b:(1, 0)
    auto Redirections = IM->extractRedirections(Inst);
    // Handle killing
    if (StoreInst *SI = dyn_cast<StoreInst>(Inst)) {
      if (Tokens.size() == 2) {
        auto Pointee = PointsToOut[Inst].getPointee(Tokens[0]);
        if (Pointee.size() == 1) {
          auto KillNode = *(Pointee.begin());
          PointsToOut[Inst].erase(KillNode);
        }
      }
    }
    // Handle special cases:
    // Handle GEP instructions
    if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(Inst)) {
      Tokens[1] = TW->getTokenWithoutIndex(Tokens[1]);
      assert(PointsToOut[Inst].getPointee(Tokens[1]).size() < 2 &&
             "GEP impl is not sound!");
      auto *Ptr = PointsToOut[Inst].getUniquePointee(Tokens[1]);
      Tokens[1] = IM->handleGEPUtil(GEP, Ptr);
      if (!Tokens[1])
        Tokens.clear();
    }
    auto PtrIdx = spatial::getPointerOperandIndex(Inst);
    if (PtrIdx > -1 && isa<GEPOperator>(Inst->getOperand(PtrIdx))) {
      assert(Tokens.size() > PtrIdx && "fix this in alias token");
      auto *Op = cast<GEPOperator>(Inst->getOperand(PtrIdx));
      if (!isa<Instruction>(Op)) {
        auto *Ptr = PointsToOut[Inst].getUniquePointee(
            TW->getToken(Op->getPointerOperand()));
        Ptr = IM->handleGEPUtil(Op, Ptr);
        PtrIdx = PtrIdx ? 0 : 1;
        Tokens[PtrIdx] = Ptr;
        if (!PtrIdx)
          Redirections[0] = 1;
        if (!Tokens[PtrIdx])
          Tokens.clear();
      }
    }
    // handle function call
    if (CallInst *CI = dyn_cast<CallInst>(Inst)) {
      if (!CI->isIndirectCall()) {
        Function &Func = *CI->getCalledFunction();
        if (!spatial::SkipFunction(Func)) {
          // pass alias information
          PointsToIn[&(Func.front().front())].merge(
              std::vector<PointsToGraph>{PointsToIn[Inst]});
          // handle pass by reference
          int ArgNum = 0;
          for (Value *Arg : CI->args()) {
            spatial::Token *ActualArg = TW->getToken(new spatial::Token(Arg));
            spatial::Token *FormalArg =
                TW->getToken(new spatial::Token(Func.getArg(ArgNum)));
            PointsToIn[&(Func.front().front())].insert(FormalArg, ActualArg, 1,
                                                       1);
            ArgNum += 1;
          }
          this->WorkList.push(&(Func.front().front()));
          this->CallGraph[&Func].insert(Inst);
          this->handleReturnValue(Inst);
        }
      }
    }
    if (&ParentBB->back() == Inst) {
      for (auto C : this->CallGraph[ParentFunc]) {
        this->WorkList.push(C);
        // this->handleReturnValue(C);
      }
    }
    if (Tokens.size() == 2) {
      // Default behavior is copy ie (1, 1)
      // for heap address in RHS make sure it is (x, 0)
      if (Tokens[1]->isMem())
        Redirections[1] = 0;
      PointsToOut[Inst].insert(Tokens[0], Tokens[1], Redirections[0],
                               Redirections[1]);
    }
    // Evaluate precision
    auto BenchVar = Bench->extract(Inst);
    if (BenchVar.size() == 2) {
      Bench->evaluate(Inst,
                      PointsToOut[Inst].getPointee(TW->getToken(BenchVar[0])),
                      PointsToOut[Inst].getPointee(TW->getToken(BenchVar[1])));
    }
  }
  void printResults(llvm::Module &M) {
    for (Function &F : M.functions()) {
      for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
        std::cout << PointsToIn[&*I] << std::endl;
        llvm::errs() << "\n[Instruction] " << *I << "\n\n";
        std::cout << PointsToOut[&*I] << std::endl;
        std::cout << "----------- " << std::endl;
      }
    }
    std::cout << *Bench;
  }
};
} // namespace FlowSensitiveAA

bool FlowSensitivePointsToAnalysisPass::runOnModule(Module &M) {
  for (Function &F : M.functions()) {
    spatial::InstNamer(F);
  }
  FlowSensitiveAA::PointsToAnalysis PA(M);
  PA.runOnWorkList();
  PA.printResults(M);
  return false;
}

char FlowSensitivePointsToAnalysisPass::ID = 0;
static RegisterPass<FlowSensitivePointsToAnalysisPass>
    X("aa-fs", "Implementation of flow-sensitive points-to analysis in LLVM",
      true, true);
