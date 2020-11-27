#include "AliasUtils.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"

/// InstNamer - sets name to the SSA temporaries
void InstNamer(llvm::Function& F) {
    for (auto& Arg : F.args()) {
        if (!Arg.hasName()) Arg.setName("arg");
    }
    for (llvm::BasicBlock& BB : F) {
        if (!BB.hasName()) BB.setName("bb");
        for (llvm::Instruction& I : BB) {
            if (!I.hasName() && !I.getType()->isVoidTy()) I.setName("i");
        }
    }
}
