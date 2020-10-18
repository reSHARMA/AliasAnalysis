#include "AliasAnalysis.h"
#include "AliasToken/AliasToken.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"

using namespace llvm;
using namespace AliasUtil;

bool AliasAnalysisPass::runOnModule(Module& M) {
    bool converged = false;
    AliasTokens AT;
    for (Function& F : M.functions()) {
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            errs() << *I << "\n";
            // TODO: Implementation
        }
    }
    return false;
}

char AliasAnalysisPass::ID = 0;
static RegisterPass<AliasAnalysisPass> X("aa-basic", "Basic implementation of alias analysis in LLVM", true, true);
