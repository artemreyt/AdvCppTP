#include "Server.hpp"
#include "Connection.hpp"
#include "Errors.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstdint>

bool get_port_safety(char *val, uint16_t &port);

const size_t PACKET_SIZE = 1048576; // 1 MB

void callback(tcp::Connection &connection, uint32_t &event) {
    std::string &buffer = connection.get_buffer();

    if (event & tcp::READ_EVENT) {
        size_t bytes = connection.read();

        if (bytes == 0) {
            connection.close();
        } else if (buffer.size() >= PACKET_SIZE || buffer.substr(buffer.size() - 4) == "stop") {
            event = tcp::WRITE_EVENT;
        }
    } else  {
        std::cout << "send to " << connection.get_dst_ip() << ":" << connection.get_dst_port()
                  << " " << buffer.size() << " bytes" << std::endl;

        bool to_close = false;
        if (buffer.substr(buffer.size() - 4) == "stop") {
            buffer.resize(buffer.size() - 4);
            to_close = true;
        }
        connection.write(buffer.data(), buffer.size());

        if (to_close) {
            connection.close();
            return;
        }
        connection.clear_buffer();
        event = tcp::READ_EVENT;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }

    uint16_t port;
    if (!get_port_safety(argv[2], port)) {
        std::cerr << "Bad port" << std::endl;
        return 1;
    }

    try {
        tcp::Server server("0.0.0.0", port, callback);


        while (true) {
            try {
                server.eventLoop();
            } catch (const tcp::epoll_error &ex) {
                std::cerr << ex.what() << std::endl;
                continue;
            } catch (const std::exception &ex) {
                std::cerr << ex.what() << std::endl;
                std::cout << "Terminating server" << std::endl;
                return 1;
            } catch (...) {
                std::cout << "Unknown error" << std::endl;
                std::cout << "Terminating server" << std::endl;
                return 1;
            }
        }
    } catch (const tcp::bad_ip_address &ex) {
        std::cerr << ex.what() << std::endl;
        std::cout << "Try another ip address" << std::endl;
    }
    return 0;
}

bool get_port_safety(char *val, uint16_t &port) {
    unsigned long long_port;

    try {
        long_port = std::stoul(val);
    } catch (const std::exception &ex) {
        return false;
    }
    if (port > UINT16_MAX) {
        return false;
    }

    port = static_cast<uint16_t>(long_port);
    return true;
}
