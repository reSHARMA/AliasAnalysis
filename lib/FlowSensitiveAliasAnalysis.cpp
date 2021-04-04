#include "FlowSensitiveAliasAnalysis.h"
#include "iostream"
#include "map"
#include "set"
#include "spatial/Benchmark/PTABenchmark.h"
#include "spatial/Graph/Graph.h"
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
  std::map<Instruction *, PointsToGraph> AliasIn, AliasOut;
  spatial::TokenWrapper TW;
  spatial::PTABenchmarkRunner *Bench;
  std::stack<llvm::Instruction *> WorkList;
  std::map<llvm::Function *, std::set<llvm::Instruction *>> CallGraph;

public:
  PointsToAnalysis(Module &M) {
    initializeWorkList(M);
    handleGlobalVar(M);
    Bench = new spatial::PTABenchmarkRunner();
  }
  void handleGlobalVar(llvm::Module &M) {
    // Handle global variables
    for (auto &G : M.getGlobalList()) {
      auto Tokens = TW.extractToken(&G);
      auto Redirections = TW.extractStatementType(&G);
      if (Tokens.size() == 2) {
        GlobalPointsToGraph.insert(Tokens[0], Tokens[1], Redirections.first,
                                   Redirections.second);
        // Handle the case when a global variable is initialized with an
        // address
        if (llvm::GlobalVariable *Constant =
                llvm::dyn_cast<GlobalVariable>(G.getInitializer())) {
          GlobalPointsToGraph.insert(Tokens[0], TW.getToken(Constant), 2, 1);
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
      PointsToGraph OldAliasInfo = AliasOut[Inst];
      runAnalysis(Inst);
      PointsToGraph NewAliasInfo = AliasOut[Inst];
      if (!(OldAliasInfo == NewAliasInfo)) {
        for (Instruction *I : spatial::GetSucc(Inst)) {
          WorkList.push(I);
        }
      }
    }
  }
  void handleReturnValue(llvm::Instruction *Inst) {
    AliasOut[Inst] = AliasIn[Inst];
    auto Tokens = TW.extractToken(Inst);
    if (CallInst *CI = dyn_cast<CallInst>(Inst)) {
      Function &Func = *CI->getCalledFunction();
      // handle return value
      if (!CI->doesNotReturn()) {
        if (ReturnInst *RI = dyn_cast<ReturnInst>(&(Func.back().back()))) {
          auto CallTokens = TW.extractToken(RI);
          if (CallTokens.size() == 1) {
            AliasOut[&(Func.back().back())].insert(Tokens[0], CallTokens[0], 1,
                                                   1);
          }
        }
      }
      // handle change made to globals
      for (auto P : AliasOut[&Func.back().back()]) {
        if (!P.first->sameFunc(&Func)) {
          for (auto X : P.second) {
            if (!X->sameFunc(&Func))
              AliasOut[Inst].insert(P.first, X);
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
      auto Tokens = TW.extractToken(Arg, ParentFunc);
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
      if (AliasOut.find(I) != AliasOut.end())
        Predecessors.push_back(AliasOut[I]);
    }
    AliasIn[Inst].merge(Predecessors);
    AliasOut[Inst] = AliasIn[Inst];
    // Extract alias tokens from the instruction
    auto Tokens = TW.extractToken(Inst);
    // Find the relative redirection between lhs and rhs
    // example for a = &b:(1, 0)
    auto Redirections = TW.extractStatementType(Inst);
    // Handle killing
    if (StoreInst *SI = dyn_cast<StoreInst>(Inst)) {
      if (Tokens.size() == 2) {
        auto Pointee = AliasOut[Inst].getPointee(Tokens[0]);
        if (Pointee.size() == 1) {
          auto KillNode = *(Pointee.begin());
          AliasOut[Inst].erase(KillNode);
        }
      }
    }
    // Handle special cases:
    // Handle GEP instructions
    if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(Inst)) {
      Tokens[1] = TW.getTokenWithoutIndex(Tokens[1]);
      assert(AliasOut[Inst].getPointee(Tokens[1]).size() < 2 &&
             "GEP impl is not sound!");
      auto *Ptr = AliasOut[Inst].getUniquePointee(Tokens[1]);
      Tokens[1] = TW.handleGEPUtil(GEP, Ptr);
      if (!Tokens[1])
        Tokens.clear();
    }
    auto PtrIdx = spatial::getPointerOperandIndex(Inst);
    if (PtrIdx > -1 && isa<GEPOperator>(Inst->getOperand(PtrIdx))) {
      assert(Tokens.size() > PtrIdx && "fix this in alias token");
      auto *Op = cast<GEPOperator>(Inst->getOperand(PtrIdx));
      if (!isa<Instruction>(Op)) {
        auto *Ptr = AliasOut[Inst].getUniquePointee(
            TW.getToken(Op->getPointerOperand()));
        Ptr = TW.handleGEPUtil(Op, Ptr);
        PtrIdx = PtrIdx ? 0 : 1;
        Tokens[PtrIdx] = Ptr;
        if (!PtrIdx)
          Redirections.first = 1;
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
          AliasIn[&(Func.front().front())].merge(
              std::vector<PointsToGraph>{AliasIn[Inst]});
          // handle pass by reference
          int ArgNum = 0;
          for (Value *Arg : CI->args()) {
            spatial::Token *ActualArg = TW.getToken(new spatial::Token(Arg));
            spatial::Token *FormalArg =
                TW.getToken(new spatial::Token(Func.getArg(ArgNum)));
            AliasIn[&(Func.front().front())].insert(FormalArg, ActualArg, 1, 1);
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
        Redirections.second = 0;
      AliasOut[Inst].insert(Tokens[0], Tokens[1], Redirections.first,
                            Redirections.second);
    }
    // Evaluate precision
    auto BenchVar = Bench->extract(Inst);
    if (BenchVar.size() == 2) {
      Bench->evaluate(Inst, AliasOut[Inst].getPointee(TW.getToken(BenchVar[0])),
                      AliasOut[Inst].getPointee(TW.getToken(BenchVar[1])));
    }
  }
  void printResults(llvm::Module &M) {
    for (Function &F : M.functions()) {
      for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
        std::cout << AliasIn[&*I];
        llvm::errs() << "\n[Instruction] " << *I << "\n\n";
        std::cout << AliasOut[&*I];
        std::cout << "----------- \n";
      }
    }
    std::cout << *Bench;
  }
};
} // namespace FlowSensitiveAA

bool FlowSensitiveAliasAnalysisPass::runOnModule(Module &M) {
  for (Function &F : M.functions()) {
    spatial::InstNamer(F);
  }
  FlowSensitiveAA::PointsToAnalysis PA(M);
  PA.runOnWorkList();
  PA.printResults(M);
  return false;
}

char FlowSensitiveAliasAnalysisPass::ID = 0;
static RegisterPass<FlowSensitiveAliasAnalysisPass>
    X("aa-fs", "Implementation of flow-sensitive alias analysis in LLVM", true,
      true);
