#include "ContextSensitiveAliasAnalysis.h"
#include "iostream"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "map"
#include "spatial/Benchmark/Benchmark.h"
#include "spatial/Graph/AliasGraph.h"
#include "spatial/Token/Alias.h"
#include "spatial/Token/AliasToken.h"
#include "spatial/Utils/CFGUtils.h"
#include "spatial/Valuecontext/ValueContext.h"
#include "stack"

using namespace llvm;
using AliasMap = spatial::AliasGraph<spatial::Alias>;

namespace ContextSensitiveAA {

class PointsToAnalysis {
   private:
    AliasMap GlobalAliasMap;
    spatial::AliasTokens AT;
    spatial::BenchmarkRunner Bench;
    std::stack<std::pair<spatial::Context, llvm::Instruction*>> WorkList;
    spatial::ValueContext<AliasMap> VC;

   public:
    PointsToAnalysis(Module& M, AliasMap BI, AliasMap Top) : VC(BI, Top) {
        initializeWorkList(M, BI);
        handleGlobalVar(M);
    }
    void handleGlobalVar(llvm::Module& M) {
        // Handle global variables
        for (auto& G : M.getGlobalList()) {
            auto Aliases = AT.extractAliasToken(&G);
            auto Redirections = AT.extractStatementType(&G);
            if (Aliases.size() == 2) {
                GlobalAliasMap.insert(Aliases[0], Aliases[1],
                                      Redirections.first, Redirections.second);
                // Handle the case when a global variable is initialized with an
                // address
                if (llvm::GlobalVariable* Constant =
                        llvm::dyn_cast<GlobalVariable>(G.getInitializer())) {
                    GlobalAliasMap.insert(Aliases[0],
                                          AT.getAliasToken(Constant), 2, 1);
                }
            }
        }
    }
    void initializeWorkList(llvm::Module& M, AliasMap BI) {
        for (Function& F : M.functions()) {
            initializeFunction(F, BI);
        }
    }
    int initializeFunction(llvm::Function& F, AliasMap InitialValue) {
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
            llvm::Instruction* Inst;
            auto Top = WorkList.top();
            std::tie(C, Inst) = Top;
            WorkList.pop();
            AliasMap OldAliasInfo = VC.getDataFlowOut[C][Inst];
            runAnalysis(Top);
            AliasMap NewAliasInfo = VC.getDataFlowOut[C][Inst];
            if (!(OldAliasInfo == NewAliasInfo)) {
                for (Instruction* I : spatial::GetSucc(Inst)) {
                    WorkList.push(std::make_pair(C, I));
                }
            }
        }
    }
    void runAnalysis(std::pair<spatial::Context, llvm::Instruction*> InstInfo) {
        llvm::Instruction* Inst;
        spatial::Context C;
        std::tie(C, Inst) = InstInfo;
        llvm::BasicBlock* ParentBB = Inst->getParent();
        llvm::Function* ParentFunc = ParentBB->getParent();
        std::vector<AliasMap> Predecessors;
        // Handle function arguments
        AliasMap ArgAliasMap;
        for (auto Arg = ParentFunc->arg_begin(); Arg != ParentFunc->arg_end();
             Arg++) {
            auto Aliases = AT.extractAliasToken(Arg, ParentFunc);
            if (Aliases.size() == 2)
                ArgAliasMap.insert(Aliases[0], Aliases[1], 1, 0);
        }
        // Only calculate aliases for global variables and arguments at the
        // start of the function
        if (&ParentBB->front() == Inst) {
            Predecessors.push_back(GlobalAliasMap);
            Predecessors.push_back(ArgAliasMap);
        }
        // Calculate control flow predecessor
        for (Instruction* I : spatial::GetPred(Inst)) {
            if (VC.getDataFlowOut[C].find(I) != VC.getDataFlowOut[C].end())
                Predecessors.push_back(VC.getDataFlowOut[C][I]);
        }
        VC.getDataFlowIn[C][Inst].merge(Predecessors);
        VC.getDataFlowOut[C][Inst] = VC.getDataFlowIn[C][Inst];
        // Find the relative redirection between lhs and rhs
        // example for a = &b:(1, 0)
        auto Redirections = AT.extractStatementType(Inst);
        // Extract alias tokens from the instruction
        auto Aliases = AT.extractAliasToken(Inst);
        // Handle killing
        if (StoreInst* SI = dyn_cast<StoreInst>(Inst)) {
            if (Aliases.size() == 2) {
                auto Pointee =
                    VC.getDataFlowOut[C][Inst].getPointee(Aliases[0]);
                if (Pointee.size() == 1) {
                    auto KillNode = *(Pointee.begin());
                    VC.getDataFlowOut[C][Inst].erase(KillNode);
                }
            }
        }
        // Handle special cases:
        // Handle GEP instructions
        if (GetElementPtrInst* GEP = dyn_cast<GetElementPtrInst>(Inst)) {
            assert(VC.getDataFlowOut[C][Inst].getPointee(Aliases[1]).size() <
                       2 &&
                   "GEP impl is not sound!");
            auto* Ptr = VC.getDataFlowOut[C][Inst].getUniquePointee(Aliases[1]);
            Aliases[1] = AT.handleGEPUtil(GEP, Ptr);
            if (!Aliases[1]) Aliases.clear();
        }
        auto PtrIdx = spatial::getPointerOperandIndex(Inst);
        if (PtrIdx > -1 && isa<GEPOperator>(Inst->getOperand(PtrIdx))) {
            assert(Aliases.size() > PtrIdx && "fix this in alias token");
            auto* Op = cast<GEPOperator>(Inst->getOperand(PtrIdx));
            if (!isa<Instruction>(Op)) {
                auto* Ptr = VC.getDataFlowOut[C][Inst].getUniquePointee(
                    AT.getAliasToken(Op->getPointerOperand()));
                Ptr = AT.handleGEPUtil(Op, Ptr);
                PtrIdx = PtrIdx ? 0 : 1;
                Aliases[PtrIdx] = Ptr;
                if (!PtrIdx) Redirections.first = 1;
                if (!Aliases[PtrIdx]) Aliases.clear();
            }
        }
        // handle function call
        if (CallInst* CI = dyn_cast<CallInst>(Inst)) {
            if (!CI->isIndirectCall()) {
                Function& Func = *CI->getCalledFunction();
                if (!spatial::SkipFunction(Func)) {
                    // handle pass by reference
                    int ArgNum = 0;
                    for (Value* Arg : CI->args()) {
                        spatial::Alias* ActualArg =
                            AT.getAliasToken(new spatial::Alias(Arg));
                        spatial::Alias* FormalArg = AT.getAliasToken(
                            new spatial::Alias(Func.getArg(ArgNum)));
                        VC.getDataFlowIn[C][Inst].insert(FormalArg, ActualArg,
                                                         1, 1);
                        ArgNum += 1;
                    }
                    // get context if previously saved
                    spatial::Context CallContext =
                        VC.getSavedContext(&Func, VC.getDataFlowIn[C][Inst]);
                    if (CallContext > -1) {
                        VC.getDataFlowOut[C][Inst] = VC.getResult(CallContext);
                    } else {
                        CallContext =
                            initializeFunction(Func, VC.getDataFlowIn[C][Inst]);
                        // update context graph
                        VC.updateContextGraph(C, CallContext, Inst);
                    }
                    // handle return value
                    if (llvm::CallInst* CI = llvm::dyn_cast<CallInst>(Inst)) {
                        if (!CI->doesNotReturn()) {
                            if (ReturnInst* RI =
                                    dyn_cast<ReturnInst>(&Func.back().back())) {
                                auto CallAliases = AT.extractAliasToken(RI);
                                if (CallAliases.size() == 1) {
                                    VC.getDataFlowOut[C][Inst].insert(
                                        Aliases[0], CallAliases[0], 1, 1);
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
                                    VC.getDataFlowOut[C][Inst].insert(P.first,
                                                                      X);
                        }
                    }
                }
            }
        }
        if (Aliases.size() == 2) {
            // Default behavior is copy ie (1, 1)
            // for heap address in RHS make sure it is (x, 0)
            if (Aliases[1]->isMem()) Redirections.second = 0;
            VC.getDataFlowOut[C][Inst].insert(Aliases[0], Aliases[1],
                                              Redirections.first,
                                              Redirections.second);
        }
        if (&ParentBB->back() == Inst) {
            VC.setResult(C, VC.getDataFlowOut[C][Inst]);
            for (auto T : VC.getContextChild(C)) {
                WorkList.push(T);
            }
        }
        // Evaluate precision
        auto BenchVar = Bench.extract(Inst);
        if (BenchVar.size() == 2) {
            Bench.evaluate(Inst,
                           VC.getDataFlowOut[C][Inst].getPointee(
                               AT.getAliasToken(BenchVar[0])),
                           VC.getDataFlowOut[C][Inst].getPointee(
                               AT.getAliasToken(BenchVar[1])));
        }
    }
    void printContextResults(llvm::Module& M) {
        for (Function& F : M.functions()) {
            for (auto C : VC.getContexts(&F)) {
                std::cout << "Function " << F.getName().str()
                          << " in Context: " << C << " => \n";
                for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E;
                     ++I) {
                    std::cout << VC.getDataFlowIn[C][&*I];
                    llvm::errs() << "\n[Instruction] " << *I << "\n\n";
                    std::cout << VC.getDataFlowOut[C][&*I];
                    std::cout << "----------- \n";
                }
            }
        }
        std::cout << Bench;
    }
};
}  // namespace ContextSensitiveAA

bool ContextSensitiveAliasAnalysisPass::runOnModule(Module& M) {
    for (Function& F : M.functions()) {
        spatial::InstNamer(F);
    }
    AliasMap BI, Top;
    ContextSensitiveAA::PointsToAnalysis PA(M, BI, Top);
    PA.runOnWorklist();
    PA.printContextResults(M);
    return false;
}

char ContextSensitiveAliasAnalysisPass::ID = 0;
static RegisterPass<ContextSensitiveAliasAnalysisPass> X(
    "aa-cs",
    "Implementation of flow-sensitive context-sensitive alias analysis in LLVM",
    true, true);
