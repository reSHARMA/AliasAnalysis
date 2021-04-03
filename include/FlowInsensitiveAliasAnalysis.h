#ifndef FLOWINSENSITIVEALIASANALYSIS_H
#define FLOWINSENSITIVEALIASANALYSIS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class FlowInsensitiveAliasAnalysisPass : public llvm::ModulePass {
public:
  static char ID;
  FlowInsensitiveAliasAnalysisPass() : ModulePass(ID) {}

  bool runOnModule(llvm::Module &M) override;
};

#endif
