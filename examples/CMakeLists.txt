set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/examples)

add_executable(single_task single_task.cpp)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(single_task fmt)
endif ()