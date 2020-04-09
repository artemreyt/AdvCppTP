//
// Created by artem on 09.04.2020.
//

#include "Server.hpp"
#include "Connection.hpp"
#include <iostream>
#include <unistd.h>

void callback(tcp::Connection &connection, uint16_t event) {
    if (event & tcp::READ_EVENT) {
        size_t bytes = connection.read();
        connection.write(connection.get_buffer().data(), bytes);
        connection.clear_buffer();
    } else  {
        std::cout << "Write event came" << std::endl;
    }
}

int main() {
    tcp::Server server("0.0.0.0", 8080, callback);
    server.eventLoop();
    return 0;
}
