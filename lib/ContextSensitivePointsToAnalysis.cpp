#include "ContextSensitivePointsToAnalysis.h"
#include "iostream"
#include "map"
#include "spatial/Benchmark/PTABenchmark.h"
#include "spatial/Graph/Graph.h"
#include "spatial/Token/Token.h"
#include "spatial/Token/TokenWrapper.h"
#include "spatial/Utils/CFGUtils.h"
#include "spatial/Valuecontext/ValueContext.h"
#include "stack"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"

using namespace llvm;
using PointsToGraph = spatial::Graph<spatial::Token>;

namespace ContextSensitiveAA {

class PointsToAnalysis {
private:
  PointsToGraph GlobalPointsToGraph;
  spatial::TokenWrapper TW;
  spatial::PTABenchmarkRunner *Bench;
  std::stack<std::pair<spatial::Context, llvm::Instruction *>> WorkList;
  spatial::ValueContext<PointsToGraph> VC;

public:
  PointsToAnalysis(Module &M, PointsToGraph BI, PointsToGraph Top)
      : VC(BI, Top) {
    initializeWorkList(M, BI);
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
  void initializeWorkList(llvm::Module &M, PointsToGraph BI) {
    for (Function &F : M.functions()) {
      initializeFunction(F, BI);
    }
  }
  int initializeFunction(llvm::Function &F, PointsToGraph InitialValue) {
    spatial::Context C = VC.initializeContext(&F, InitialValue);
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
      if (I == inst_begin(F)) {
        VC.getDataFlowIn[C][&*I] = InitialValue;
        WorkList.push(std::make_pair(C, &*I));
      }
    }
    return C;
  }
  void runOnWorklist() {
    while (!WorkList.empty()) {
      spatial::Context C;
      llvm::Instruction *Inst;
      auto Top = WorkList.top();
      std::tie(C, Inst) = Top;
      WorkList.pop();
      PointsToGraph OldPointsToInfo = VC.getDataFlowOut[C][Inst];
      runAnalysis(Top);
      PointsToGraph NewPointsToInfo = VC.getDataFlowOut[C][Inst];
      if (!(OldPointsToInfo == NewPointsToInfo)) {
        for (Instruction *I : spatial::GetSucc(Inst)) {
          WorkList.push(std::make_pair(C, I));
        }
      }
    }
  }
  void runAnalysis(std::pair<spatial::Context, llvm::Instruction *> InstInfo) {
    llvm::Instruction *Inst;
    spatial::Context C;
    std::tie(C, Inst) = InstInfo;
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
    // Only calculate aliases for global variables and arguments at the
    // start of the function
    if (&ParentBB->front() == Inst) {
      Predecessors.push_back(GlobalPointsToGraph);
      Predecessors.push_back(ArgPointsToGraph);
    }
    // Calculate control flow predecessor
    for (Instruction *I : spatial::GetPred(Inst)) {
      if (VC.getDataFlowOut[C].find(I) != VC.getDataFlowOut[C].end())
        Predecessors.push_back(VC.getDataFlowOut[C][I]);
    }
    VC.getDataFlowIn[C][Inst].merge(Predecessors);
    VC.getDataFlowOut[C][Inst] = VC.getDataFlowIn[C][Inst];
    // Find the relative redirection between lhs and rhs
    // example for a = &b:(1, 0)
    auto Redirections = TW.extractStatementType(Inst);
    // Extract alias tokens from the instruction
    auto Tokens = TW.extractToken(Inst);
    // Handle killing
    if (StoreInst *SI = dyn_cast<StoreInst>(Inst)) {
      if (Tokens.size() == 2) {
        auto Pointee = VC.getDataFlowOut[C][Inst].getPointee(Tokens[0]);
        if (Pointee.size() == 1) {
          auto KillNode = *(Pointee.begin());
          VC.getDataFlowOut[C][Inst].erase(KillNode);
        }
      }
    }
    // Handle special cases:
    // Handle GEP instructions
    if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(Inst)) {
      Tokens[1] = TW.getTokenWithoutIndex(Tokens[1]);
      assert(VC.getDataFlowOut[C][Inst].getPointee(Tokens[1]).size() < 2 &&
             "GEP impl is not sound!");
      auto *Ptr = VC.getDataFlowOut[C][Inst].getUniquePointee(Tokens[1]);
      Tokens[1] = TW.handleGEPUtil(GEP, Ptr);
      if (!Tokens[1])
        Tokens.clear();
    }
    auto PtrIdx = spatial::getPointerOperandIndex(Inst);
    if (PtrIdx > -1 && isa<GEPOperator>(Inst->getOperand(PtrIdx))) {
      assert(Tokens.size() > PtrIdx && "fix this in alias token");
      auto *Op = cast<GEPOperator>(Inst->getOperand(PtrIdx));
      if (!isa<Instruction>(Op)) {
        auto *Ptr = VC.getDataFlowOut[C][Inst].getUniquePointee(
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
          // handle pass by reference
          int ArgNum = 0;
          for (Value *Arg : CI->args()) {
            spatial::Token *ActualArg = TW.getToken(new spatial::Token(Arg));
            spatial::Token *FormalArg =
                TW.getToken(new spatial::Token(Func.getArg(ArgNum)));
            VC.getDataFlowIn[C][Inst].insert(FormalArg, ActualArg, 1, 1);
            ArgNum += 1;
          }
          // get context if previously saved
          spatial::Context CallContext =
              VC.getSavedContext(&Func, VC.getDataFlowIn[C][Inst]);
          if (CallContext > -1) {
            VC.getDataFlowOut[C][Inst] = VC.getResult(CallContext);
          } else {
            CallContext = initializeFunction(Func, VC.getDataFlowIn[C][Inst]);
            // update context graph
            VC.updateContextGraph(C, CallContext, Inst);
          }
          // handle return value
          if (llvm::CallInst *CI = llvm::dyn_cast<CallInst>(Inst)) {
            if (!CI->doesNotReturn()) {
              if (ReturnInst *RI = dyn_cast<ReturnInst>(&Func.back().back())) {
                auto CallTokens = TW.extractToken(RI);
                if (CallTokens.size() == 1) {
                  VC.getDataFlowOut[C][Inst].insert(Tokens[0], CallTokens[0], 1,
                                                    1);
                }
              }
            }
          }
          // handle change made to globals
          auto TempResult = VC.getDataFlowOut[C][Inst];
          VC.getDataFlowOut[C][Inst] = VC.getTop();
          for (auto P : TempResult) {
            if (!P.first->sameFunc(&Func)) {
              for (auto X : P.second)
                if (!X->sameFunc(&Func))
                  VC.getDataFlowOut[C][Inst].insert(P.first, X);
            }
          }
        }
      }
    }
    if (Tokens.size() == 2) {
      // Default behavior is copy ie (1, 1)
      // for heap address in RHS make sure it is (x, 0)
      if (Tokens[1]->isMem())
        Redirections.second = 0;
      VC.getDataFlowOut[C][Inst].insert(
          Tokens[0], Tokens[1], Redirections.first, Redirections.second);
    }
    if (&ParentBB->back() == Inst) {
      VC.setResult(C, VC.getDataFlowOut[C][Inst]);
      for (auto T : VC.getContextChild(C)) {
        WorkList.push(T);
      }
    }
    // Evaluate precision
    auto BenchVar = Bench->extract(Inst);
    if (BenchVar.size() == 2) {
      Bench->evaluate(
          Inst, VC.getDataFlowOut[C][Inst].getPointee(TW.getToken(BenchVar[0])),
          VC.getDataFlowOut[C][Inst].getPointee(TW.getToken(BenchVar[1])));
    }
  }
  void printContextResults(llvm::Module &M) {
    for (Function &F : M.functions()) {
      for (auto C : VC.getContexts(&F)) {
        std::cout << "Function " << F.getName().str() << " in Context: " << C
                  << " => \n";
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
          std::cout << VC.getDataFlowIn[C][&*I];
          llvm::errs() << "\n[Instruction] " << *I << "\n\n";
          std::cout << VC.getDataFlowOut[C][&*I];
          std::cout << "----------- \n";
        }
      }
    }
    std::cout << *Bench;
  }
};
} // namespace ContextSensitiveAA

bool ContextSensitivePointsToAnalysisPass::runOnModule(Module &M) {
  for (Function &F : M.functions()) {
    spatial::InstNamer(F);
  }
  PointsToGraph BI, Top;
  ContextSensitiveAA::PointsToAnalysis PA(M, BI, Top);
  PA.runOnWorklist();
  PA.printContextResults(M);
  return false;
}

char ContextSensitivePointsToAnalysisPass::ID = 0;
static RegisterPass<ContextSensitivePointsToAnalysisPass>
    X("aa-cs",
      "Implementation of flow-sensitive context-sensitive points-to analysis "
      "in LLVM",
      true, true);
