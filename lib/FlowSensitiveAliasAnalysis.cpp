#include "FlowSensitiveAliasAnalysis.h"
#include "AliasBench/Benchmark.h"
#include "AliasGraph/AliasGraph.h"
#include "AliasToken/Alias.h"
#include "AliasToken/AliasToken.h"
#include "CFGUtils/CFGUtils.h"
#include "iostream"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "map"

using namespace llvm;
using AliasMap = AliasGraphUtil::AliasGraph<AliasUtil::Alias>;

bool FlowSensitiveAliasAnalysisPass::runOnModule(Module& M) {
    std::map<Instruction*, AliasMap> AliasIn, AliasOut;
    AliasUtil::AliasTokens AT;
    BenchmarkUtil::BenchmarkRunner Bench;
    std::map<llvm::Function*, std::vector<llvm::Value*>> FuncRetValue;
    // Handle global variables
    AliasMap GlobalAliasMap;
    for (auto& G : M.getGlobalList()) {
        auto Aliases = AT.extractAliasToken(&G);
        auto Redirections = AT.extractStatementType(&G);
        if (Aliases.size() == 2) {
            GlobalAliasMap.insert(Aliases[0], Aliases[1], Redirections.first,
                                  Redirections.second);
            // Handle the case when a global variable is initialized with an
            // address
            if (llvm::GlobalVariable* Constant =
                    llvm::dyn_cast<GlobalVariable>(G.getInitializer())) {
                GlobalAliasMap.insert(Aliases[0], AT.getAliasToken(Constant), 2,
                                      1);
            }
        }
    }
    for (Function& F : M.functions()) {
        CFGUtils::InstNamer(F);
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            std::vector<AliasMap> Predecessors;
            // Handle function arguments
            AliasMap ArgAliasMap;
            for (auto Arg = F.arg_begin(); Arg != F.arg_end(); Arg++) {
                auto Aliases = AT.extractAliasToken(Arg, &F);
                if (Aliases.size() == 2)
                    ArgAliasMap.insert(Aliases[0], Aliases[1], 1, 0);
            }
            // Only calculate aliases for global variables and arguments at the
            // start of the function
            if (I == inst_begin(F)) {
                Predecessors.push_back(GlobalAliasMap);
                Predecessors.push_back(ArgAliasMap);
            }
            // Calculate control flow predecessor
            for (Instruction* I : CFGUtils::GetPred(&*I)) {
                if (AliasOut.find(I) != AliasOut.end())
                    Predecessors.push_back(AliasOut[I]);
            }
            AliasIn[&*I].merge(Predecessors);
            AliasOut[&*I] = AliasIn[&*I];
            // Extract alias tokens from the instruction
            auto Aliases = AT.extractAliasToken(&*I);
            // Handle killing
            if (StoreInst* Inst = dyn_cast<StoreInst>(&*I)) {
                if (Aliases.size() == 2) {
                    auto Pointee = AliasOut[&*I].getPointee(Aliases[0]);
                    if (Pointee.size() == 1) {
                        auto KillNode = *(Pointee.begin());
                        AliasOut[&*I].erase(KillNode);
                    }
                }
            }
            // Handle special cases:
            // Handle GEP instructions
            if (GetElementPtrInst* Inst = dyn_cast<GetElementPtrInst>(&*I)) {
                for (auto A : AliasOut[&*I].getPointee(Aliases[1])) {
                    AliasUtil::Alias* FieldVal = new AliasUtil::Alias(A);
                    FieldVal->setIndex(Inst);
                    FieldVal = AT.getAliasToken(FieldVal);
                    AliasOut[&*I].insert(Aliases[0], FieldVal, 1, 0);
                }
                // clear Aliases to avoid confusions
                Aliases.clear();
            }
            // handle return instructions
            if (ReturnInst* Inst = dyn_cast<ReturnInst>(&*I)) {
                Value* RetVal = Inst->getReturnValue();
                if (RetVal && !isa<llvm::ConstantInt>(RetVal))
                    FuncRetValue[&F].push_back(RetVal);
            }
            // handle function call
            if (CallInst* Inst = dyn_cast<CallInst>(&*I)) {
                if (!Inst->isIndirectCall()) {
                    Function& Func = *Inst->getCalledFunction();
                    if (!CFGUtils::SkipFunction(Func)) {
                        // handle return value
                        if (!Inst->doesNotReturn()) {
                            for (Value* V : FuncRetValue[&Func]) {
                                AliasUtil::Alias* RetVal =
                                    AT.getAliasToken(new AliasUtil::Alias(V));
                                for (auto P :
                                     AliasOut[cast<Instruction>(V)].getPointee(
                                         RetVal))
                                    AliasOut[&*I].insert(Aliases[0], P, 1, 0);
                            }
                        }
                        // handle pass by reference
                        int ArgNum = 0;
                        for (Value* Arg : Inst->args()) {
                            AliasUtil::Alias* ActualArg =
                                AT.getAliasToken(new AliasUtil::Alias(Arg));
                            AliasUtil::Alias* FormalArg = AT.getAliasToken(
                                new AliasUtil::Alias(Func.getArg(ArgNum)));
                            AliasIn[&(Func.front().front())].insert(
                                FormalArg, ActualArg, 1, 0);
                            ArgNum += 1;
                        }
                        // handle change made to globals
                        for (auto P : AliasOut[&Func.back().back()]) {
                            if (!P.first->sameFunc(&Func)) {
                                AliasOut[&*I].insert(P.first, P.second);
                            }
                        }
                    }
                }
            }
            // Find the relative redirection between lhs and rhs
            // example for a = &b:(1, 0)
            auto Redirections = AT.extractStatementType(&*I);
            if (Aliases.size() == 2) {
                // Default behavior is copy ie (1, 1)
                // for heap address in RHS make sure it is (x, 0)
                if (Aliases[1]->isMem()) Redirections.second = 0;
                AliasOut[&*I].insert(Aliases[0], Aliases[1], Redirections.first,
                                     Redirections.second);
            }
            // Evaluate precision
            auto BenchVar = Bench.extract(&*I);
            if (BenchVar.size() == 2) {
                Bench.evaluate(
                    &*I,
                    AliasOut[&*I].getPointee(AT.getAliasToken(BenchVar[0])),
                    AliasOut[&*I].getPointee(AT.getAliasToken(BenchVar[1])));
            }
        }
    }
    for (Function& F : M.functions()) {
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            std::cout << AliasIn[&*I];
            llvm::errs() << "\n[Instruction] " << *I << "\n\n";
            std::cout << AliasOut[&*I];
            std::cout << "-------------------------------------\n";
        }
    }
    std::cout << Bench;
    return false;
}

char FlowSensitiveAliasAnalysisPass::ID = 0;
static RegisterPass<FlowSensitiveAliasAnalysisPass> X(
    "aa-fs", "Implementation of flow-sensitive alias analysis in LLVM", true,
    true);
