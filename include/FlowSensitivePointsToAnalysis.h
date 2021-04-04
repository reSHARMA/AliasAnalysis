#ifndef FLOWSENSITIVEPOINTSTOANALYSIS_H
#define FLOWSENSITIVEPOINTSTOANALYSIS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class FlowSensitivePointsToAnalysisPass : public llvm::ModulePass {
public:
  static char ID;
  FlowSensitivePointsToAnalysisPass() : ModulePass(ID) {}

  bool runOnModule(llvm::Module &M) override;
};

#endif
