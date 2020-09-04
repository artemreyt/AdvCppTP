cmake_minimum_required(VERSION 3.10)

project(KVReader)

add_executable(FileGenerator FileGenerator/FileGenerator.cpp src/MmapIO/ForwardReader.cpp)
target_link_libraries(FileGenerator Logger)
target_include_directories(FileGenerator PUBLIC include)

add_library(KVReader src/KVReader/KVReader.cpp src/IndexMaker/IndexMaker.cpp src/MmapIO/ForwardReader.cpp src/MmapIO/RandomWriter.cpp src/utils/utils.cpp)
target_include_directories(KVReader PUBLIC include include/KVReader include/IndexMaker include/MmapIO include/utils)
target_compile_features(KVReader PUBLIC cxx_std_17)
target_link_libraries(KVReader PRIVATE Logger Descriptor)