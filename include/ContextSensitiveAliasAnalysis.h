#ifndef CONTEXTSENSITIVEALIASANALYSIS_H
#define CONTEXTSENSITIVEALIASANALYSIS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class ContextSensitiveAliasAnalysisPass : public llvm::ModulePass {
public:
  static char ID;
  ContextSensitiveAliasAnalysisPass() : ModulePass(ID) {}

  bool runOnModule(llvm::Module &M) override;
};

#endif
