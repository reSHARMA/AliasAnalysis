#ifndef ALIASANALYSIS_H
#define ALIASANALYSIS_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"

#define DEBUG_TYPE "aa-basic"

class AliasAnalysisPass : public llvm::ModulePass {
   public:
    static char ID;
    AliasAnalysisPass() : ModulePass(ID) {}

    bool runOnModule(llvm::Module& M) override;
};

#endif
