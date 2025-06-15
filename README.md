## LLVM?
LLVM (Low Level Virtual Machine) is a collection of modular and reusable compiler and toolchain technologies. It provides a framework for building compilers, optimizers, and code generators. LLVM is used in many production compilers, including Clang and the Clang++ compilers which supports various programming languages.

## Project Overview
This project implements a custom LLVM pass that detects the usage of unsafe functions like `scanf` and many more in C/C++ code. The pass analyzes LLVM IR (Intermediate Representation) and reports when it finds calls to `scanf` or related functions. The compiled build uses the `PreservedAnalyses` class to prevent anything from the IR modification but soon will start optimizing IR to produce production ready object files

## LLVM Passes
LLVM passes are those pieces of code which could be used with the LLVM, thanks to the modular architecture rather than the mololithic `gcc` compilers. When a new exploit is discovered or reported, the whole community should wait for the next release of the `gcc` build (SDLE Lifecycle) `gcc-14.3` (Current point of time) to patch the exploit which is stedious in the current growing trend. LLVM offers a modular architecture most popular for the AOT (Ahead Of Time) and the JIT (Just In Time) compilers to fit in our own optimization techniques for a faster and efficient build.

## Prerequisites
- LLVM 19
- CMake (version 3.13 or higher)
- C++ compiler with C++17 support
- libzstd development package

### Dependencies for target OS
On Debian/Ubuntu/Kali Linux:
```bash
sudo apt-get update
sudo apt-get install llvm-19 libzstd-dev cmake make
```

## Project Structure
```
.
├── CMakeLists.txt      # CMake build configuration
├── main.cpp           # LLVM pass implementation
├── test.c            # Test C program
└── test.ll           # LLVM IR of test.c
```

## Building the Pass
1. Create a build directory and navigate to it:
```bash
mkdir build && cd build
```

2. Generate build files with CMake:
```bash
cmake ..
```

3. Build the pass:
```bash
make
```

This will generate `ScanfChecker.so` in your build directory.

## Shared objects in linux
After building the project, a file named `ScanfChecker.so` is generated in the build directory. This is a shared object file, a dynamic library on Linux that contains the compiled logic for checking or analyzing scanf usage in C/C++ programs.

This file is intended to be used as a plugin or runtime checker. For example, it may be loaded by Clang, LLVM, or other tools to inspect source code and identify unsafe or incorrect uses of scanf. This helps ensure better code safety, especially in projects where input validation is critical. The actual shared object is uploaded to the releases section

## How the Pass Works
The pass (`ScanfCheckerPass`) works by:
1. Analyzing each function in the LLVM IR
2. Looking for function calls
3. Checking if the called function's name contains "scanf"
4. Reporting any scanf usage found

## Testing the Pass
1. First, compile your C program to LLVM IR:
```bash
clang -S -emit-llvm test.c -o test.ll
```

2. Run the pass using the LLVM opt tool:
```bash
opt -load-pass-plugin ./ScanfChecker.so -passes="function(scanf-checker)" -disable-output test.ll
```

### Example Output
```
ScanfChecker plugin loaded for LLVM 19!
Registering scanf-checker pass in pipeline
>> Running ScanfChecker on function: main
    Found call to: printf
    Found call to: __isoc99_scanf
==> SCANF DETECTED in function: main (calls __isoc99_scanf)
    Found call to: printf
    Found call to: __isoc99_scanf
==> SCANF DETECTED in function: main (calls __isoc99_scanf)
    Found call to: printf
```

## Troubleshooting

### Common Issues

1. **Pass not running:**
   - Make sure the function in your IR doesn't have the `optnone` attribute
   - Check that you're using the correct LLVM version

2. **Build errors:**
   - Ensure all dependencies are installed
   - Check that your C++ compiler supports C++17
   - Verify that LLVM 19 is properly installed

3. **Plugin loading errors:**
   - Make sure the path to `ScanfChecker.so` is correct
   - Check that the plugin was built with the same LLVM version as the opt tool

## Extending the Pass
To modify the pass to detect other functions or add more analysis:
1. Edit `main.cpp`
2. Rebuild using the steps in the "Building the Pass" section
3. Test with the opt tool
