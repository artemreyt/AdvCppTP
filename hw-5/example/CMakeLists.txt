add_executable(example.out example.cpp)
target_link_libraries(example.out shmem)
target_compile_features(example.out PRIVATE cxx_std_17)