# cmake_minimum_required(VERSION 3.22)
project(WMFConverter)

# set cpp version to c++20
set(CMAKE_CXX_STANDARD 20)

# Define compile target
add_executable(WMFConverter main.cpp)

# Add library paths and headers, if necessary
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/path/to/gdiplus_header)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/path/to/gdiplus_library)

# Link library to compile target
target_link_libraries(WMFConverter PRIVATE Gdiplus)