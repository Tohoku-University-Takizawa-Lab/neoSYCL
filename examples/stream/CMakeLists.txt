add_executable(
        stream
        Stream.h
        SYCLStream.h
        main.cpp
        SYCLStream.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(stream fmt)
endif ()