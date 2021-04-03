#ifndef FLOWSENSITIVEALIASANALYSIS_H
#define FLOWSENSITIVEALIASANALYSIS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class FlowSensitiveAliasAnalysisPass : public llvm::ModulePass {
public:
  static char ID;
  FlowSensitiveAliasAnalysisPass() : ModulePass(ID) {}

  bool runOnModule(llvm::Module &M) override;
};

#endif
