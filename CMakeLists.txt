cmake_minimum_required(VERSION 3.29)
project(randomized_incremental_convex_hull)

set(CMAKE_CXX_STANDARD 20)

# Find libcurl
find_package(CURL REQUIRED)

# Find JsonCpp
find_package(JsonCpp REQUIRED)

# Add the executable
add_executable(randomized_incremental_convex_hull main.cpp)

# Link libraries
target_link_libraries(randomized_incremental_convex_hull PRIVATE CURL::libcurl JsonCpp::JsonCpp)