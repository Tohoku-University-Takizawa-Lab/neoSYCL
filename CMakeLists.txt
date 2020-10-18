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
    # find VEOS headers and libs
    set(VEO_HEADER_PATH "/opt/nec/ve/veos/include" CACHE PATH "veo headers path")
    set(VEO_LIB_PATH "/opt/nec/ve/veos/lib64" CACHE PATH "veo libs path")
    message(STATUS "Enable SX-Aurora support")
    message(STATUS "Use veo headers path: ${VEO_HEADER_PATH}")
    message(STATUS "Use veo libs path: ${VEO_LIB_PATH}")

    include_directories(${VEO_HEADER_PATH})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${VEO_LIB_PATH} -Wl,-rpath=${VEO_LIB_PATH} -lveo")
endif ()

# use pthread
find_package(Threads REQUIRED)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
#find_package(OpenMP REQUIRED)

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