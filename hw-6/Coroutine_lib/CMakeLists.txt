cmake_minimum_required(VERSION 3.16)
project(coroutine_example)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-O3)

add_library(coroutine src/coroutine.cpp)

add_executable(coroutine_example src/main.cpp)
target_link_libraries(coroutine_example coroutine pthread)
target_include_directories(coroutine PUBLIC include)
