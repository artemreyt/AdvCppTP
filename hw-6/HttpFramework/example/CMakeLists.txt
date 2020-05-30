add_executable(example_server example.cpp)
target_compile_options(example_server PUBLIC -O0 -ggdb3)
target_link_libraries(example_server HttpFramework)