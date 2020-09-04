cmake_minimum_required(VERSION 3.10)

add_library(Descriptor src/Descriptor.cpp)
target_include_directories(Descriptor PUBLIC include)
target_compile_features(Descriptor PUBLIC cxx_std_17)