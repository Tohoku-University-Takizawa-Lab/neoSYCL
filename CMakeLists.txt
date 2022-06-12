cmake_minimum_required(VERSION 3.4)
project("neoSYCL" LANGUAGES CXX VERSION 0.5.0)
add_library(neoSYCL INTERFACE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

option(BUILD_KERNEL_OUTLINER "build sycl kernel outliner" ON)
option(BUILD_VE "Enable SX-Aurora support" OFF)


if (BUILD_KERNEL_OUTLINER)
    add_subdirectory(src)
endif ()


if (BUILD_VE)
    ADD_DEFINITIONS(-DBUILD_VE)
    # find VEOS headers and libs
    set(VEO_HEADER_PATH "/opt/nec/ve/veos/include" CACHE PATH "veo headers path")
    set(VEO_LIB_PATH "/opt/nec/ve/veos/lib64" CACHE PATH "veo libs path")
    message(STATUS "Enable SX-Aurora support")
    message(STATUS "Use veo headers path: ${VEO_HEADER_PATH}")
    message(STATUS "Use veo libs path:  ${VEO_LIB_PATH}")

    include_directories(${VEO_HEADER_PATH} ${CMAKE_SOURCE_DIR}/include/neoSYCL/)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${VEO_LIB_PATH} -Wl,-rpath=${VEO_LIB_PATH} -lveo")
endif ()

# debug info
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    message(STATUS "Enable debug mode")
    ADD_DEFINITIONS(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

include_directories(include)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

target_include_directories(
  neoSYCL
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/neoSYCL DESTINATION include)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/CL DESTINATION include)
