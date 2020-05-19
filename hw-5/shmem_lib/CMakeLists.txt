cmake_minimum_required(VERSION 3.10)

add_library(shmem src/ShmemErrors.cpp)
target_include_directories(shmem PUBLIC include)
target_compile_features(shmem PRIVATE cxx_std_17)
target_link_libraries(shmem utils pthread)

add_library(utils src/utils/SemLock.cpp)
target_include_directories(utils PUBLIC include/utils)
target_compile_features(utils PRIVATE cxx_std_17)