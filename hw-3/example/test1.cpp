#include "Connection.hpp"
#include "Server.hpp"
#include "Errors.hpp"
#include <iostream>
#include <string>

int main() {
    tcp::Server server("127.0.0.1", 7777);

    while ( true ) {
        std::string ip_client;
        uint16_t port_client;

        tcp::Connection serverEndPoint = server.accept();
        serverEndPoint.get_connection_info(&ip_client, &port_client, nullptr, nullptr);

        std::cout << "Started connection with " << ip_client << ":" << port_client << std::endl;

        try {
            char c;
            while (true) {
                serverEndPoint.readExact(&c, 1);
                std::cout << c;
            }
        } catch (const tcp::error &err) {
            std::cerr << err.what() << "(likely connection is closed)" << std::endl;
        } catch (const std::exception& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << "Exit server" << std::endl;
            break;
        }

    }
    return 0;
}

