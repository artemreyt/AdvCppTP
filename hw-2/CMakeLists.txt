cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Logger src/BaseLogger.cpp src/Logger.cpp src/LoggerErrors.cpp)
target_include_directories(Logger PUBLIC include)
target_compile_features(Logger PUBLIC cxx_std_17)

add_subdirectory(example)