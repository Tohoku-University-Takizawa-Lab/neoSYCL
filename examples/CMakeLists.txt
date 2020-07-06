add_executable(
        main
        sequential_vector.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(main fmt)
endif ()

add_subdirectory(stream)