cmake_minimum_required(VERSION 3.28.1)
project(randomized_incremental_convex_hull)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

# Find libcurl
find_package(CURL REQUIRED)

# Find JsonCpp
find_package(JsonCpp REQUIRED)

# Add the executable for the visualizer test
add_executable(test_visualizer
        test_visualizer.cpp
        include/visualizer_helper.h
)

# Add the sequential convex hull executable
add_executable(sequential_incremental_convex_hull
        test_sequential_visualizer.cpp
        src/sequential_convex_hull.cpp
        include/sequential_convex_hull.h
        include/visualizer_helper.h
)

# Add the parallel convex hull executable
add_executable(parallel_incremental_convex_hull
        test_parallel_visualizer.cpp
        src/parallel_convex_hull.cpp
        include/parallel_convex_hull.h
        include/visualizer_helper.h
)

# Link libraries to all executables
target_link_libraries(test_visualizer PRIVATE CURL::libcurl JsonCpp::JsonCpp)
target_link_libraries(sequential_incremental_convex_hull PRIVATE CURL::libcurl JsonCpp::JsonCpp)
target_link_libraries(parallel_incremental_convex_hull PRIVATE CURL::libcurl JsonCpp::JsonCpp)
