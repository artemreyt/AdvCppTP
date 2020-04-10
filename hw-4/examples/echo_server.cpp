#include "Server.hpp"
#include "Connection.hpp"
#include "Errors.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>

const size_t PACKET_SIZE = 8;

void callback(tcp::Connection &connection, uint32_t &event) {
    const std::string &buffer = connection.get_buffer();

    if (event & tcp::READ_EVENT) {
        connection.read();
        if (buffer.size() >= PACKET_SIZE)
            event = tcp::WRITE_EVENT;
    } else  {
        connection.write(buffer.data(), buffer.size());
        connection.clear_buffer();
        event = tcp::READ_EVENT;
    }
}

int main() {
    tcp::Server server("0.0.0.0", 8080, callback);
    while (true) {
        try {
            server.eventLoop();
        } catch (tcp::error &ex) {
            std::cerr << ex.what() << std::endl;
            continue;
        } catch(std::exception &ex) {
            std::cerr << ex.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception" << std::endl;
        }
        break;
    }
    return 0;
}
