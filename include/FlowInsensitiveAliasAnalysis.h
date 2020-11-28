#ifndef FLOWINSENSITIVEALIASANALYSIS_H
#define FLOWINSENSITIVEALIASANALYSIS_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"

class FlowInsensitiveAliasAnalysisPass : public llvm::ModulePass {
   public:
    static char ID;
    FlowInsensitiveAliasAnalysisPass() : ModulePass(ID) {}

    bool runOnModule(llvm::Module& M) override;
};

#endif
