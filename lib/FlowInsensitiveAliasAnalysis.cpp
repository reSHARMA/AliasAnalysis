#include "FlowInsensitiveAliasAnalysis.h"
#include "iostream"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "spatial/Benchmark/PTABenchmark.h"
#include "spatial/Graph/AliasGraph.h"
#include "spatial/Token/Alias.h"
#include "spatial/Token/AliasToken.h"
#include "spatial/Utils/CFGUtils.h"

using namespace llvm;
bool FlowInsensitiveAliasAnalysisPass::runOnModule(Module& M) {
    spatial::AliasTokens AT;
    spatial::AliasGraph<spatial::Alias> AG;
    spatial::PTABenchmarkRunner *Bench = new spatial::PTABenchmarkRunner();
    // Handle global variables
    for (auto& G : M.getGlobalList()) {
        auto Aliases = AT.extractAliasToken(&G);
        auto Redirections = AT.extractStatementType(&G);
        if (Aliases.size() == 2) {
            AG.insert(Aliases[0], Aliases[1], Redirections.first,
                      Redirections.second);
            // Handle the case when a global variable is initialized with an
            // address
            if (llvm::GlobalVariable* Constant =
                    llvm::dyn_cast<GlobalVariable>(G.getInitializer())) {
                AG.insert(Aliases[0], AT.getAliasToken(Constant), 2, 1);
            }
        }
    }
    for (Function& F : M.functions()) {
        spatial::InstNamer(F);
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            // Extract alias tokens from the instruction
            auto Aliases = AT.extractAliasToken(&*I);
            // Find the relative redirection between lhs and rhs
            // example for a = &b:(1, 0)
            auto Redirections = AT.extractStatementType(&*I);
            // Handle special cases:
            // Handle GEP instructions
            if (GetElementPtrInst* Inst = dyn_cast<GetElementPtrInst>(&*I)) {
                Aliases[1] = AT.getAliasTokenWithoutIndex(Aliases[1]);
                assert(AG.getPointee(Aliases[1]).size() < 2 &&
                       "GEP impl is not sound!");
                auto* Ptr = AG.getUniquePointee(Aliases[1]);
                Aliases[1] = AT.handleGEPUtil(Inst, Ptr);
                if (!Aliases[1]) Aliases.clear();
            }
            auto PtrIdx = spatial::getPointerOperandIndex(&*I);
            if (PtrIdx > -1 && isa<GEPOperator>((*I).getOperand(PtrIdx))) {
                assert(Aliases.size() > PtrIdx && "fix this in alias token");
                auto* Op = cast<GEPOperator>((*I).getOperand(PtrIdx));
                if (!isa<Instruction>(Op)) {
                    auto* Ptr = AG.getUniquePointee(
                        AT.getAliasToken(Op->getPointerOperand()));
                    Ptr = AT.handleGEPUtil(Op, Ptr);
                    PtrIdx = PtrIdx ? 0 : 1;
                    Aliases[PtrIdx] = Ptr;
                    if (!PtrIdx) Redirections.first = 1;
                    if (!Aliases[PtrIdx]) Aliases.clear();
                }
            }
            if (Aliases.size() == 2) {
                // Default behavior is copy ie (1, 1)
                // for heap address in RHS make sure it is (x, 0)
                if (Aliases[1]->isMem()) Redirections.second = 0;
                AG.insert(Aliases[0], Aliases[1], Redirections.first,
                          Redirections.second);
            }
            // Evaluate precision
            auto BenchVar = Bench->extract(&*I);
            if (BenchVar.size() == 2) {
                Bench->evaluate(&*I,
                               AG.getPointee(AT.getAliasToken(BenchVar[0])),
                               AG.getPointee(AT.getAliasToken(BenchVar[1])));
            }
        }
    }
    std::cout << AG;
    std::cout << *Bench;
    return false;
}

char FlowInsensitiveAliasAnalysisPass::ID = 0;
static RegisterPass<FlowInsensitiveAliasAnalysisPass> X(
    "aa-basic", "Basic implementation of alias analysis in LLVM", true, true);
