add_executable(echo_server.out echo_server.cpp)
target_link_libraries(echo_server.out tcp)

add_executable(client.out client.cpp)
target_link_libraries(client.out tcp)