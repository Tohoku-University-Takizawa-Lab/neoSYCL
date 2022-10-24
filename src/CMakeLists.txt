# find Clang
find_package(Clang REQUIRED)
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${CLANG_DEFINITIONS})
message(STATUS "Found Clang")
message(STATUS "Clang_DIR ${Clang_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${Clang_DIR}")

# include all headers here
include_directories(${CMAKE_SOURCE_DIR}/include/neoSYCL/kout)

set(SOURCE_FILES
  main.cpp
  KoutVisitor.cpp
  KoutPrinterHelper.cpp
  VarDeclFinder.cpp
)

# add executable file


add_executable(kout ${SOURCE_FILES})

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

target_link_libraries(kout PRIVATE clang-cpp)

configure_file(nsc++.in nsc++)

install(TARGETS kout DESTINATION bin)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/nsc++ DESTINATION bin)
