cmake_minimum_required(VERSION 3.10)

project(FileReader)

add_executable(FileGenerator src/FileGenerator/FileGenerator.cpp)
target_link_libraries(FileGenerator Logger)
target_include_directories(FileGenerator PUBLIC include)

add_library(FileReader src/FileReader/FileReader.cpp)
target_include_directories(FileReader PUBLIC include include/FileReader)