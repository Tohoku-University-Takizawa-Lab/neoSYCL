set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/examples)

function(add_executable_sycl name)
    message("Build target: ${name}")
    add_executable(${name} ${ARGN})
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        target_link_libraries(${name} fmt)
    endif ()
endfunction()

add_executable_sycl(single_task single_task.cpp)
add_executable_sycl(sequential_vector sequential_vector.cpp)


