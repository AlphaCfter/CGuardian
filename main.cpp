#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"

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

extern "C" PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "ScanfChecker",
        "v1.0",
        [](PassBuilder &PB) {
            errs() << "ScanfChecker plugin loaded\n";
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "scanf-checker") {
                        errs() << "Registering scanf-checker pass in pipeline\n";
                        FPM.addPass(ScanfCheckerPass());
                        return true;
                    }
                    return false;
                });
        }
    };
}