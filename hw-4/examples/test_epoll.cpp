//
// Created by artem on 09.04.2020.
//

#include "Server.hpp"
#include "Connection.hpp"
#include <iostream>
#include <unistd.h>

void callback(tcp::Connection &connection, tcp::event_t event) {
    if (uint16_t(event) & uint16_t(tcp::event_t::READ_EVENT)) {
        static char buffer[2];
        size_t bytes = connection.read(buffer, 2);
        connection.write(buffer, bytes);
        sleep(1);
    } else  {
        std::cout << "Write event came" << std::endl;
    }
}

int main() {
    tcp::Server server("0.0.0.0", 8080, callback);
    server.eventLoop();
    return 0;
}
