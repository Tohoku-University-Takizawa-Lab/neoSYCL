
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/examples)

add_subdirectory(cpu)
add_subdirectory(bfs)
if (BUILD_VE)
    add_subdirectory(ve)
    add_subdirectory(nbody)
    add_subdirectory(stream)
endif ()
