cmake_minimum_required(VERSION 3.13)
project(ScanfChecker LANGUAGES CXX)

find_package(LLVM REQUIRED CONFIG)

add_library(ScanfChecker MODULE main.cpp)

target_include_directories(ScanfChecker PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(ScanfChecker PRIVATE ${LLVM_DEFINITIONS})

set_target_properties(ScanfChecker PROPERTIES
    PREFIX ""
    COMPILE_FLAGS "-fno-rtti"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
