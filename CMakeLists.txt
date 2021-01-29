cmake_minimum_required(VERSION 3.4)
project(neoSYCL C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_VE "Enable SX-Aurora support" OFF)
option(BUILD_TESTING "Enable build tests" OFF)

include_directories(include)


if (BUILD_VE)
    ADD_DEFINITIONS(-DBUILD_VE)
    include_directories(/usr/src/debug/veoffload-aveo-2.7.3/build/include)
    set(CMAKE_EXE_LINKER_FLAGS "-lpthread -L/opt/nec/ve/veos/lib64 -Wl,-rpath=/opt/nec/ve/veos/lib64 -lveo")
endif ()


# use pthread
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
#find_package(Threads REQUIRED)

# debug info
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    message(STATUS "Enable debug mode")
    ADD_DEFINITIONS(-DDEBUG)
    include_directories(third_party/fmt/include)
    add_subdirectory(third_party/fmt)
endif ()

if (SYNC_MODE)
    message(STATUS "Enable sync mode")
    ADD_DEFINITIONS(-DSYNC)
endif ()

add_subdirectory(examples)

# add tests
if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()