cmake_minimum_required(VERSION 3.4)
project(sycl C CXX)

set(CMAKE_CXX_STANDARD 17)


include_directories(
        include
)

option(BUILD_VE "Enable SX-Aurora support" ON)

if (BUILD_VE)
    # find VEOS headers and libs
    set(VEO_HEADER_PATH "/opt/nec/ve/veos/include" CACHE PATH "veo headers path")
    set(VEO_LIB_PATH "/opt/nec/ve/veos/lib64" CACHE PATH "veo libs path")
    message(STATUS "Enable SX-Aurora support")
    message(STATUS "Use veo headers path: ${VEO_HEADER_PATH}, use veo libs path: ${VEO_LIB_PATH}")

    include_directories(${VEO_HEADER_PATH})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${VEO_LIB_PATH} -Wl,-rpath=${VEO_LIB_PATH} -lveo")
endif ()

# use pthread
find_package(Threads REQUIRED)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

# debug info
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
    include_directories(third_party/fmt/include)
    add_subdirectory(third_party/fmt)
endif ()

if (SYNC_MODE)
    ADD_DEFINITIONS(-DSYNC)
endif ()

add_subdirectory(src)

# add tests
if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()