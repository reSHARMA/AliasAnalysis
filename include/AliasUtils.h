#ifndef ALIASUTILS_H
#define ALIASUTILS_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "vector"

void InstNamer(llvm::Function &F);
bool SkipFunction(llvm::Function &F);
std::vector<llvm::Instruction *> GetPred(llvm::Instruction *);

#endif
