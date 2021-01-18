#include "FlowSensitiveAliasAnalysis.h"
#include "iostream"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "map"
#include "set"
#include "spatial/Benchmark/Benchmark.h"
#include "spatial/Graph/AliasGraph.h"
#include "spatial/Token/Alias.h"
#include "spatial/Token/AliasToken.h"
#include "spatial/Utils/CFGUtils.h"
#include "stack"

using namespace llvm;
using AliasMap = spatial::AliasGraph<spatial::Alias>;

namespace FlowSensitiveAA {

class PointsToAnalysis {
   private:
    AliasMap GlobalAliasMap;
    std::map<Instruction*, AliasMap> AliasIn, AliasOut;
    spatial::AliasTokens AT;
    spatial::BenchmarkRunner Bench;
    std::stack<llvm::Instruction*> WorkList;
    std::map<llvm::Function*, std::set<llvm::Instruction*>> CallGraph;

   public:
    PointsToAnalysis(Module& M) {
        initializeWorkList(M);
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
    void initializeWorkList(llvm::Module& M) {
        for (Function& F : M.functions()) {
            for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E;
                 ++I) {
                if (I == inst_begin(F)) {
                    WorkList.push(&*I);
                }
            }
        }
    }
    void runOnWorkList() {
        while (!WorkList.empty()) {
            Instruction* Inst = WorkList.top();
            WorkList.pop();
            AliasMap OldAliasInfo = AliasOut[Inst];
            runAnalysis(Inst);
            AliasMap NewAliasInfo = AliasOut[Inst];
            if (!(OldAliasInfo == NewAliasInfo)) {
                for (Instruction* I : spatial::GetSucc(Inst)) {
                    WorkList.push(I);
                }
            }
        }
    }
    void handleReturnValue(llvm::Instruction* Inst) {
        AliasOut[Inst] = AliasIn[Inst];
        auto Aliases = AT.extractAliasToken(Inst);
        if (CallInst* CI = dyn_cast<CallInst>(Inst)) {
            Function& Func = *CI->getCalledFunction();
            // handle return value
            if (!CI->doesNotReturn()) {
                if (ReturnInst* RI =
                        dyn_cast<ReturnInst>(&(Func.back().back()))) {
                    auto CallAliases = AT.extractAliasToken(RI);
                    if (CallAliases.size() == 1) {
                        AliasOut[&(Func.back().back())].insert(
                            Aliases[0], CallAliases[0], 1, 1);
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
    void runAnalysis(llvm::Instruction* Inst) {
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
        // Only calculate aliases for global variables and arguments at
        // the
        // start of the function
        if (&ParentBB->front() == Inst) {
            Predecessors.push_back(GlobalAliasMap);
            Predecessors.push_back(ArgAliasMap);
        }
        // Calculate control flow predecessor
        for (Instruction* I : spatial::GetPred(Inst)) {
            if (AliasOut.find(I) != AliasOut.end())
                Predecessors.push_back(AliasOut[I]);
        }
        AliasIn[Inst].merge(Predecessors);
        AliasOut[Inst] = AliasIn[Inst];
        // Extract alias tokens from the instruction
        auto Aliases = AT.extractAliasToken(Inst);
        // Find the relative redirection between lhs and rhs
        // example for a = &b:(1, 0)
        auto Redirections = AT.extractStatementType(Inst);
        // Handle killing
        if (StoreInst* SI = dyn_cast<StoreInst>(Inst)) {
            if (Aliases.size() == 2) {
                auto Pointee = AliasOut[Inst].getPointee(Aliases[0]);
                if (Pointee.size() == 1) {
                    auto KillNode = *(Pointee.begin());
                    AliasOut[Inst].erase(KillNode);
                }
            }
        }
        // Handle special cases:
        // Handle GEP instructions
        if (GetElementPtrInst* GEP = dyn_cast<GetElementPtrInst>(Inst)) {
            assert(AliasOut[Inst].getPointee(Aliases[1]).size() < 2 &&
                   "GEP impl is not sound!");
            auto* Ptr = AliasOut[Inst].getUniquePointee(Aliases[1]);
            Aliases[1] = AT.handleGEPUtil(GEP, Ptr);
            if (!Aliases[1]) Aliases.clear();
        }
        auto PtrIdx = spatial::getPointerOperandIndex(Inst);
        if (PtrIdx > -1 && isa<GEPOperator>(Inst->getOperand(PtrIdx))) {
            assert(Aliases.size() > PtrIdx && "fix this in alias token");
            auto* Op = cast<GEPOperator>(Inst->getOperand(PtrIdx));
            if (!isa<Instruction>(Op)) {
                auto* Ptr = AliasOut[Inst].getUniquePointee(
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
                    // pass alias information
                    AliasIn[&(Func.front().front())].merge(
                        std::vector<AliasMap>{AliasIn[Inst]});
                    // handle pass by reference
                    int ArgNum = 0;
                    for (Value* Arg : CI->args()) {
                        spatial::Alias* ActualArg =
                            AT.getAliasToken(new spatial::Alias(Arg));
                        spatial::Alias* FormalArg = AT.getAliasToken(
                            new spatial::Alias(Func.getArg(ArgNum)));
                        AliasIn[&(Func.front().front())].insert(
                            FormalArg, ActualArg, 1, 1);
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
        if (Aliases.size() == 2) {
            // Default behavior is copy ie (1, 1)
            // for heap address in RHS make sure it is (x, 0)
            if (Aliases[1]->isMem()) Redirections.second = 0;
            AliasOut[Inst].insert(Aliases[0], Aliases[1], Redirections.first,
                                  Redirections.second);
        }
        // Evaluate precision
        auto BenchVar = Bench.extract(Inst);
        if (BenchVar.size() == 2) {
            Bench.evaluate(
                Inst, AliasOut[Inst].getPointee(AT.getAliasToken(BenchVar[0])),
                AliasOut[Inst].getPointee(AT.getAliasToken(BenchVar[1])));
        }
    }
    void printResults(llvm::Module& M) {
        for (Function& F : M.functions()) {
            for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E;
                 ++I) {
                std::cout << AliasIn[&*I];
                llvm::errs() << "\n[Instruction] " << *I << "\n\n";
                std::cout << AliasOut[&*I];
                std::cout << "----------- \n";
            }
        }
        std::cout << Bench;
    }
};
}  // namespace FlowSensitiveAA

bool FlowSensitiveAliasAnalysisPass::runOnModule(Module& M) {
    for (Function& F : M.functions()) {
        spatial::InstNamer(F);
    }
    FlowSensitiveAA::PointsToAnalysis PA(M);
    PA.runOnWorkList();
    PA.printResults(M);
    return false;
}

char FlowSensitiveAliasAnalysisPass::ID = 0;
static RegisterPass<FlowSensitiveAliasAnalysisPass> X(
    "aa-fs", "Implementation of flow-sensitive alias analysis in LLVM", true,
    true);
