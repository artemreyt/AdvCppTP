#include "Server.hpp"
#include "Connection.hpp"
#include "Errors.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstdint>

uint16_t get_port_safety(char *val);

const size_t PACKET_SIZE = 8;

void callback(tcp::Connection &connection, uint32_t &event) {
    const std::string &buffer = connection.get_buffer();

    if (event & tcp::READ_EVENT) {
        size_t bytes = connection.read();

        if (bytes == 0) {
            connection.close();
        } else if (buffer.size() >= PACKET_SIZE) {
            event = tcp::WRITE_EVENT;
        }
    } else  {
        connection.write(buffer.data(), buffer.size());
        connection.clear_buffer();
        event = tcp::READ_EVENT;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }

    uint16_t port = get_port_safety(argv[1]);

    try {
        tcp::Server server("0.0.0.0", 8080, callback);


        while (true) {
            try {
                server.eventLoop();
            } catch (const tcp::epoll_error &ex) {
                std::cerr << ex.what() << std::endl;
                continue;
            } catch (const std::exception &ex) {
                std::cerr << ex.what() << std::endl;
                std::cout << "Terminating server" << std::endl;
                exit(EXIT_FAILURE);
            } catch (...) {
                std::cout << "Unknown error" << std::endl;
                std::cout << "Terminating server" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    } catch (const tcp::bad_ip_address &ex) {
        std::cout << ex.what() << std::endl;
        std::cout << "Try another ip address" << std::endl;
    }
    return 0;
}

uint16_t get_port_safety(char *val) {
    unsigned long port;

    try {
        port = std::stoul(val);
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    if (port > UINT16_MAX) {
        std::cout << "port must be <= " << UINT16_MAX << std::endl;
        exit(EXIT_FAILURE);
    }
    return static_cast<uint16_t>(port);
}
