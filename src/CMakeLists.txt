# find Clang
find_package(Clang REQUIRED CONFIG HINTS ${LLVM_INSTALL_PREFIX}/lib/cmake/clang
)
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${CLANG_DEFINITIONS})
message(STATUS "Clang_FOUND ${Clang_FOUND}")
message(STATUS "Clang_DIR ${Clang_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${Clang_DIR}")

# find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM_FOUND ${LLVM_FOUND}")
message(STATUS "LLVM_DIR ${LLVM_DIR}")
message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})
add_definitions(
-D__STDC_LIMIT_MACROS
-D__STDC_CONSTANT_MACROS
)
include_directories(${LLVM_INCLUDE_DIRS})
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

# include all headers here
include_directories(${CMAKE_SOURCE_DIR}/include/neoSYCL/kout)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
endif ()

set(SOURCE_FILES 
  main.cpp
  KoutVisitor.cpp 
  KoutPrinterHelper.cpp 
  VarDeclFinder.cpp 
 )


# add executable file 
add_executable(kout ${SOURCE_FILES})

#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

target_link_libraries(kout
  clangTooling
  clangFrontendTool
  clangFrontend
  clangDriver
  clangSerialization
  clangCodeGen
  clangParse
  clangSema
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
  clangAnalysis
  clangARCMigrate
  clangRewrite 
  clangRewriteFrontend
  clangEdit 
  clangAST 
  clangLex 
  clangBasic 
  clang-cpp
  clang
  z
)

# link clang libraries
target_link_libraries(kout
  LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
  LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
  #LLVMX86AsmPrinter # MC, Support, X86Utils
  LLVMX86Info # MC, Support, Target
  #LLVMX86Utils # Core, Support
  LLVMipo
  LLVMScalarOpts
  LLVMInstCombine
  LLVMTransformUtils
  #LLVMipa
  LLVMAnalysis
  LLVMTarget
  LLVMOption # Support
  LLVMMCParser # MC, Support
  LLVMMC # Object, Support
  LLVMObject # BitReader, Core, Support
  LLVMBitReader # Core, Support
  LLVMCore # Support
  LLVMSupport
)

add_custom_target(nsc++.py
ALL 
COMMAND "sed" 
"-e" "\"s|@NEOSYCL_DIR@|NEOSYCL_DIR=\'${CMAKE_INSTALL_PREFIX}/\'|g\""
"-e" "\"s|@CLANG_DIR@|CLANG_DIR=\'${Clang_DIR}/../../clang/${LLVM_PACKAGE_VERSION}/\'|g\""
"-e" "\"s|@VEOS_DIR@|VEOS_DIR=\'${VEO_HEADER_PATH}\'|g\""
 "${PROJECT_SOURCE_DIR}/src/nsc++.py.in" ">" "nsc++.py"
)

install (TARGETS kout DESTINATION bin)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/nsc++.py DESTINATION bin)
