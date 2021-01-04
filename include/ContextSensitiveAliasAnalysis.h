#ifndef CONTEXTSENSITIVEALIASANALYSIS_H
#define CONTEXTSENSITIVEALIASANALYSIS_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"

class ContextSensitiveAliasAnalysisPass : public llvm::ModulePass {
   public:
    static char ID;
    ContextSensitiveAliasAnalysisPass() : ModulePass(ID) {}

    bool runOnModule(llvm::Module& M) override;
};

#endif
