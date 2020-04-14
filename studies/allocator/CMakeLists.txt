cmake_minimum_required(VERSION 3.10)


add_executable(alloc.out allocator.cpp)
target_compile_features(alloc.out PRIVATE cxx_std_17)