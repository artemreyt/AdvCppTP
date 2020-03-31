add_executable(parent.out src/parent.cpp)
add_executable(child.out src/child.cpp)

target_link_libraries(parent.out proc Protocol)
target_link_libraries(child.out proc Protocol)