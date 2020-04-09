//
// Created by artem on 09.04.2020.
//

#include "Server.hpp"
#include "Connection.hpp"
#include <iostream>

void callback(tcp::Connection &connection, tcp::event_t event) {
    if (event == tcp::event_t::READ_EVENT) {
        static char buffer[1024];
        size_t bytes = connection.read(buffer, 1024);
        connection.write(buffer, bytes);
    } else  {
        std::cout << "Write event came" << std::endl;
    }
}

int main() {
    tcp::Server server("0.0.0.0", 8080, callback);
    server.eventLoop();
    return 0;
}
