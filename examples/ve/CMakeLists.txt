add_executable(
        sequential_vector
        sequential_vector.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(sequential_vector fmt)
endif ()

