cmake_minimum_required(VERSION 3.10)

project(tcp)

add_library(tcp src/Server.cpp src/Errors.cpp src/Connection.cpp src/utils.cpp)
target_include_directories(tcp PUBLIC include)
target_compile_features(tcp PUBLIC cxx_std_17)
target_link_libraries(tcp proc Descriptor)

add_subdirectory(Process_lib)
add_subdirectory(example)