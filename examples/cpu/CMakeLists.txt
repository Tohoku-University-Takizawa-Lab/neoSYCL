add_executable(
        cpu_sequential_vector
        sequential_vector.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(cpu_sequential_vector fmt)
endif ()

