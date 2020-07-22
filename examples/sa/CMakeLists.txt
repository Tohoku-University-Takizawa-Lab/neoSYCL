
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/examples)

add_subdirectory(cpu)
if (BUILD_VE)
    add_subdirectory(ve)
    add_subdirectory(stream)
endif ()
