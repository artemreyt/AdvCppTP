
add_library(Protocol STATIC src/Protocol.cpp)
target_include_directories(Protocol PUBLIC include)
target_link_libraries(Protocol proc)


add_executable(parent.out src/parent.cpp)
add_executable(child.out src/child.cpp)
target_link_libraries(parent.out proc Protocol)
target_link_libraries(child.out proc Protocol)