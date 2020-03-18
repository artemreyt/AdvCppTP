cmake_minimum_required(VERSION 3.10)

project(proc)

set(SOURCE_LIB src/Process.cpp src/utils.cpp)

add_library(proc STATIC ${SOURCE_LIB})
target_include_directories(proc PUBLIC include)
target_compile_options(proc PRIVATE -Wall -Wextra -Werror)
target_compile_features(proc PUBLIC cxx_std_17)

add_subdirectory(example)
add_subdirectory(lib)