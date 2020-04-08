#include "Connection.hpp"
#include "Server.hpp"
#include "Errors.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

int main() {
    tcp::Server server("0.0.0.0", 7777);

    while ( true ) {
        tcp::Connection serverEndPoint = server.accept();


        std::string ip_client = serverEndPoint.get_dst_ip(), ip_server = serverEndPoint.get_src_ip();
        uint16_t port_client = serverEndPoint.get_dst_port(), port_server = serverEndPoint.get_src_port();

        std::cout << "Started connection with " << ip_client << ":" << port_client
                    << " from " << ip_server << ":" << port_server << std::endl;

        try {
            char c;
            while (true) {
                serverEndPoint.readExact(&c, 1);
                std::cout << c;
            }
        } catch (const tcp::error &err) {
            std::cerr << err.what() << " (likely connection is closed)" << std::endl;
        } catch (const std::exception& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << "Exit server" << std::endl;
            break;
        }

    }
    return 0;
}

