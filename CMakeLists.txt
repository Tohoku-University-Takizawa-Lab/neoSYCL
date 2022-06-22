cmake_minimum_required(VERSION 3.4)
project("neoSYCL" LANGUAGES CXX VERSION 0.5.0)
add_library(neoSYCL INTERFACE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_KERNEL_OUTLINER "Build SYCL kernel outliner" ON)
option(BUILD_VE "Enable SX-Aurora support" OFF)

if (BUILD_VE)
    ADD_DEFINITIONS(-DBUILD_VE)
    # find VEOS headers and libs
    set(VEO_DIR "/opt/nec/ve/veos" CACHE PATH "VEO installation path")
    set(VEO_HEADER_PATH "${VEO_DIR}/include" CACHE PATH "VEO headers path")
    set(VEO_LIB_PATH "${VEO_DIR}/lib64" CACHE PATH "VEO libs path")
    message(STATUS "Enable SX-Aurora support")
    message(STATUS "Use veo headers path: ${VEO_HEADER_PATH}")
    message(STATUS "Use veo libs path:  ${VEO_LIB_PATH}")

    include_directories(${VEO_HEADER_PATH} ${CMAKE_SOURCE_DIR}/include/neoSYCL/)
endif ()

if (BUILD_KERNEL_OUTLINER)
    add_subdirectory(src)
endif ()

include_directories(include)

target_include_directories(
    neoSYCL
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/neoSYCL DESTINATION include)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/CL DESTINATION include)
