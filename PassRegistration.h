/*
 * Created by Ajith Kumar on 6/15/25
 * A utility header containing macros to simplify and reduce boilerplate
 * for registering LLVM passes via the new PassBuilder pipeline system.
*/

/*
 * Examples:
 *       void registerMyPasses(PassBuilder &PB) {
 *       PB.registerPipelineParsingCallback(
 *       [](StringRef Name, FunctionPassManager &FPM,
 *          ArrayRef<PassBuilder::PipelineElement>) {
 *           REGISTER_FUNCTION_PASS("scanf-checker", ScanfCheckerPass);
 *           return false;
 *          });
 *      }
 *      DEFINE_PLUGIN("ScanfChecker", "v1.0", registerMyPasses)
 *
 */

#ifndef PASSREGISTRATION_H
#define PASSREGISTRATION_H
#pragma once

#include "llvm/Passes/PassBuilder.h"


// Macro to register the pass from the name declared in the caller function
#define REGISTER_FUNCTION_PASS(NAME, TYPE)           \
if (Name == NAME) {                              \
FPM.addPass(TYPE());                         \
return true;                                 \
}

/*
 * Macro that prints the plugin within the header file
 * for more verbosity and robustness though the output stream
 */
#define PRINT_PLUGIN_BANNER(NAME, VERSION)                                     \
llvm::errs() << "Loaded plugin: " << NAME << " (" << VERSION << ")\n";

/*
 * Macro to define the enrty point of the plugin
 */
#define DEFINE_PLUGIN(PLUGIN_NAME, VERSION, REGISTER_CALLBACK)               \
extern "C" llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {             \
    return {                                                                 \
        LLVM_PLUGIN_API_VERSION,                                             \
        PLUGIN_NAME,                                                         \
        VERSION,                                                             \
        [](llvm::PassBuilder &PB) {                                          \
        PRINT_PLUGIN_BANNER(PLUGIN_NAME, VERSION);                           \
        REGISTER_CALLBACK(PB);                                               \
    }                                                                        \
};                                                                           \
}

#endif //PASSREGISTRATION_H
