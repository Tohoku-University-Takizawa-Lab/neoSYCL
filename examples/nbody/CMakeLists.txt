add_executable(
        nbody
        nbody.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(nbody fmt)
endif ()
