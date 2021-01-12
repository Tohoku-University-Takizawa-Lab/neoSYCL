
find_package(Boost REQUIRED)

add_executable(
        bfs
        bfs.cpp
)


add_executable(
        bfs_sycl
        bfs_sycl.cpp
)


add_executable(
        graph_gen
        inputGen/graphgen.cpp
)