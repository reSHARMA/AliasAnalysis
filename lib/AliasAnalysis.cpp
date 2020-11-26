#include "AliasAnalysis.h"
#include "AliasBench/Benchmark.h"
#include "AliasGraph/AliasGraph.h"
#include "AliasToken/Alias.h"
#include "AliasToken/AliasToken.h"
#include "iostream"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"

using namespace llvm;

bool AliasAnalysisPass::runOnModule(Module& M) {
    AliasUtil::AliasTokens AT;
    AliasGraphUtil::AliasGraph<AliasUtil::Alias> AG;
    BenchmarkUtil::BenchmarkRunner Bench;
    for (Function& F : M.functions()) {
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            // Extract alias tokens from the instruction
            auto Aliases = AT.extractAliasToken(&*I);
            // Find the relative redirection between lhs and rhs
            // example for a = &b:(1, 0)
            auto Redirections = AT.extractStatementType(&*I);
            if (Aliases.size() == 2) {
                // Default behavior is copy ie (1, 1)
                // for heap address in RHS make sure it is (x, 0)
                if (Aliases[1]->isMem()) Redirections.second = 0;
                AG.insert(Aliases[0], Aliases[1], Redirections.first,
                          Redirections.second);
            }
            // Evaluate precision
            auto BenchVar = Bench.extract(&*I);
            if (BenchVar.size() == 2) {
                Bench.evaluate(&*I,
                               AG.getPointee(AT.getAliasToken(BenchVar[0])),
                               AG.getPointee(AT.getAliasToken(BenchVar[1])));
            }
        }
    }
    std::cout << AG;
    std::cout << Bench;
    return false;
}

char AliasAnalysisPass::ID = 0;
static RegisterPass<AliasAnalysisPass> X(
    "aa-basic", "Basic implementation of alias analysis in LLVM", true, true);
