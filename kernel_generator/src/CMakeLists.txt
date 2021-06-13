
# add executable file
add_executable(
        clang_tool

        single_task.cpp
        parallel_task.cpp
        kernel.cpp
        clang_tool.cpp
        ve_kernel_translator.cpp
)

# link clang libraries
target_link_libraries(clang_tool

        clangFrontend
        clangSerialization
        clangDriver
        clangParse
        clangSema
        clangAnalysis
        clangAST
        clangBasic
        clangEdit
        clangLex
        clangTooling
        clangASTMatchers
        clangAnalysis
        clangEdit
        clangAST
        clangLex
        clangRewrite
        clangToolingCore

        ${llvm_libs}

        fmt
        )



