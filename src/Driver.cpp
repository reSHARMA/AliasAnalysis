#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "AliasAnalysis.h"

using namespace llvm;

int main(int argc, char **argv) {
    if (argc < 1) return 1;
    LLVMContext Context;
    SMDiagnostic Error;
    std::unique_ptr<Module> M = parseIRFile(argv[1], Error, Context);
    AliasAnalysisPass * AAP = new AliasAnalysisPass();
    AAP -> runOnModule(*M);
    return 0;
}
