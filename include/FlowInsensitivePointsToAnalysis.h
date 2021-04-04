#ifndef FLOWINSENSITIVEPOINTSTOANALYSIS_H
#define FLOWINSENSITIVEPOINTSTOANALYSIS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class FlowInsensitivePointsToAnalysisPass : public llvm::ModulePass {
public:
  static char ID;
  FlowInsensitivePointsToAnalysisPass() : ModulePass(ID) {}

  bool runOnModule(llvm::Module &M) override;
};

#endif
