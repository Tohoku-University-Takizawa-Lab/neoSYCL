
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
get_filename_component(DEPS_ROOT "${PROJECT_BINARY_DIR}/deps" ABSOLUTE)

include(ExtProjectUtils)

find_package(Threads REQUIRED)


# Will download external CMakeable project from git repo, branch "master" and install it in $DEPS_ROOT
# This also will create "googletest.git" target, which we'll use as dependency for our test project
ExtProjectGit("https://github.com/google/googletest.git" "master" ${DEPS_ROOT} CMAKE_ARGS "${_OPT_CMAKE_ARGS}")

include_directories("${DEPS_ROOT}/include")
link_directories("${DEPS_ROOT}/lib")
link_directories("${DEPS_ROOT}/lib64")


#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(COMMON_LIBS fmt)

#-------------------
# Test
#-------------------
enable_testing()
include_directories(${COMMON_INCLUDES})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

# from list of files we'll create tests test_name.cpp -> test_name
foreach (_test_file ${TEST_SRC_FILES})
    get_filename_component(_test_name ${_test_file} NAME_WE)
    add_executable(${_test_name} ${_test_file})
    add_dependencies(${_test_name} "googletest.git")
    target_link_libraries(${_test_name} gtestd gtest_maind ${COMMON_LIBS} ${CMAKE_THREAD_LIBS_INIT})
    add_test(${_test_name} ${_test_name})
    set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
endforeach ()