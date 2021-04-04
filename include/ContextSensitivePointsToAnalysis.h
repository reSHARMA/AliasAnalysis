#ifndef CONTEXTSENSITIVEPOINTSTOANALYSIS_H
#define CONTEXTSENSITIVEPOINTSTOANALYSIS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class ContextSensitivePointsToAnalysisPass : public llvm::ModulePass {
public:
  static char ID;
  ContextSensitivePointsToAnalysisPass() : ModulePass(ID) {}

  bool runOnModule(llvm::Module &M) override;
};

#endif
