#include "FlowInsensitiveAliasAnalysis.h"
#include "iostream"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "spatial/Benchmark/PTABenchmark.h"
#include "spatial/Graph/AliasGraph.h"
#include "spatial/Token/Token.h"
#include "spatial/Token/TokenWrapper.h"
#include "spatial/Utils/CFGUtils.h"

using namespace llvm;
bool FlowInsensitiveAliasAnalysisPass::runOnModule(Module& M) {
    spatial::TokenWrapper TW;
    spatial::AliasGraph<spatial::Token> AG;
    spatial::PTABenchmarkRunner *Bench = new spatial::PTABenchmarkRunner();
    // Handle global variables
    for (auto& G : M.getGlobalList()) {
        auto Tokens = TW.extractToken(&G);
        auto Redirections = TW.extractStatementType(&G);
        if (Tokens.size() == 2) {
            AG.insert(Tokens[0], Tokens[1], Redirections.first,
                      Redirections.second);
            // Handle the case when a global variable is initialized with an
            // address
            if (llvm::GlobalVariable* Constant =
                    llvm::dyn_cast<GlobalVariable>(G.getInitializer())) {
                AG.insert(Tokens[0], TW.getToken(Constant), 2, 1);
            }
        }
    }
    for (Function& F : M.functions()) {
        spatial::InstNamer(F);
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            // Extract alias tokens from the instruction
            auto Tokens = TW.extractToken(&*I);
            // Find the relative redirection between lhs and rhs
            // example for a = &b:(1, 0)
            auto Redirections = TW.extractStatementType(&*I);
            // Handle special cases:
            // Handle GEP instructions
            if (GetElementPtrInst* Inst = dyn_cast<GetElementPtrInst>(&*I)) {
                Tokens[1] = TW.getTokenWithoutIndex(Tokens[1]);
                assert(AG.getPointee(Tokens[1]).size() < 2 &&
                       "GEP impl is not sound!");
                auto* Ptr = AG.getUniquePointee(Tokens[1]);
                Tokens[1] = TW.handleGEPUtil(Inst, Ptr);
                if (!Tokens[1]) Tokens.clear();
            }
            auto PtrIdx = spatial::getPointerOperandIndex(&*I);
            if (PtrIdx > -1 && isa<GEPOperator>((*I).getOperand(PtrIdx))) {
                assert(Tokens.size() > PtrIdx && "fix this in alias token");
                auto* Op = cast<GEPOperator>((*I).getOperand(PtrIdx));
                if (!isa<Instruction>(Op)) {
                    auto* Ptr = AG.getUniquePointee(
                        TW.getToken(Op->getPointerOperand()));
                    Ptr = TW.handleGEPUtil(Op, Ptr);
                    PtrIdx = PtrIdx ? 0 : 1;
                    Tokens[PtrIdx] = Ptr;
                    if (!PtrIdx) Redirections.first = 1;
                    if (!Tokens[PtrIdx]) Tokens.clear();
                }
            }
            if (Tokens.size() == 2) {
                // Default behavior is copy ie (1, 1)
                // for heap address in RHS make sure it is (x, 0)
                if (Tokens[1]->isMem()) Redirections.second = 0;
                AG.insert(Tokens[0], Tokens[1], Redirections.first,
                          Redirections.second);
            }
            // Evaluate precision
            auto BenchVar = Bench->extract(&*I);
            if (BenchVar.size() == 2) {
                Bench->evaluate(&*I,
                               AG.getPointee(TW.getToken(BenchVar[0])),
                               AG.getPointee(TW.getToken(BenchVar[1])));
            }
        }
    }
    std::cout << AG;
    std::cout << *Bench;
    return false;
}

char FlowInsensitiveAliasAnalysisPass::ID = 0;
static RegisterPass<FlowInsensitiveAliasAnalysisPass> X(
    "aa-basic", "Basic implementation of alias analysis in LLVM", true, true);
