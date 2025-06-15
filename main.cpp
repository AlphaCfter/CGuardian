#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "PassRegistration.h"

using namespace llvm;

struct ScanfCheckerPass : PassInfoMixin<ScanfCheckerPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        errs() << ">> Running ScanfChecker on function: " << F.getName() << "\n";

        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                if (auto *CB = dyn_cast<CallBase>(&I)) {
                    Function *Callee = CB->getCalledFunction();
                    if (Callee) {
                        StringRef FuncName = Callee->getName();
                        errs() << "    Found call to: " << FuncName << "\n";
                        if (FuncName.contains("scanf")) {
                            errs() << "==> SCANF DETECTED in function: " << F.getName()
                                   << " (calls " << FuncName << ")\n";
                        }
                    }
                }
            }
        }
        return PreservedAnalyses::all();
    }
};

void registerMyPasses(PassBuilder &PB) {
    PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
            REGISTER_FUNCTION_PASS("scanf-checker", ScanfCheckerPass);
            return false;
        });
}

DEFINE_PLUGIN("ScanfChecker", "v1.0", registerMyPasses)