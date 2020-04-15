add_executable(example_int_str.out example_int_str.cpp)
target_link_libraries(example_int_str.out shmem)
target_compile_features(example_int_str.out PRIVATE cxx_std_17)

add_executable(example_str_int.out example_str_int.cpp)
target_link_libraries(example_str_int.out shmem)
target_compile_features(example_str_int.out PRIVATE cxx_std_17)