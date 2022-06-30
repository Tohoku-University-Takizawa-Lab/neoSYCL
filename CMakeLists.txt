cmake_minimum_required(VERSION 3.10)

project(neoSYCL LANGUAGES CXX VERSION 0.5.0)
add_library(neoSYCL INTERFACE)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_KERNEL_OUTLINER "Build SYCL kernel outliner" ON)
option(BUILD_VE "Enable SX-Aurora TSUBASA support" OFF)

if (BUILD_VE)
    add_definitions(-DBUILD_VE)
    # find VEOS headers and libs
    set(VEO_DIR "/opt/nec/ve/veos" CACHE PATH "VEO installation path")
    find_path(VEO_HEADER_PATH ve_offload.h HINTS ${VEO_DIR} PATH_SUFFIXES include)
    find_path(VEO_LIB_PATH libveo.so HINTS ${VEO_DIR} PATH_SUFFIXES lib64)

    message(STATUS "Enable SX-Aurora support")
    message(STATUS "Use VEO headers path: ${VEO_HEADER_PATH}")
    message(STATUS "Use VEO libs path:    ${VEO_LIB_PATH}")

    include_directories(${VEO_HEADER_PATH} include/neoSYCL/)
endif ()

if (BUILD_KERNEL_OUTLINER)
    add_subdirectory(src)
endif ()

include_directories(include)

target_include_directories(
    neoSYCL
    INTERFACE $<BUILD_INTERFACE:include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/neoSYCL DESTINATION include)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/CL DESTINATION include)
