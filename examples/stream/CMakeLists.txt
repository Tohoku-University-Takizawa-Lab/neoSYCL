add_executable(
        ve-stream
        Stream.h
        SYCLStream.h
        main.cpp
        SYCLStream.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(ve-stream fmt)
endif ()