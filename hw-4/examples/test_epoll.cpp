#include "Server.hpp"
#include "Connection.hpp"
#include "Errors.hpp"
#include <stdexcept>
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
