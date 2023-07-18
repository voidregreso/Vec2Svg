cmake_minimum_required(VERSION 3.10)

# set the project name
project(Vector2Svg)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(tinyxml2_DIR "${CMAKE_SOURCE_DIR}/tinyxml2/lib/cmake/tinyxml2")

# add the executable
add_executable(Vector2Svg vec2svg.cpp)

# find the tinyxml2 library
find_package(tinyxml2 REQUIRED)

# link the tinyxml2 library
target_link_libraries(Vector2Svg PRIVATE tinyxml2::tinyxml2)
