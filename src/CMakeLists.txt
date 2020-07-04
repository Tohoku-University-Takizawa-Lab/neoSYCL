add_executable(
        main
        main.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(main fmt)
endif ()