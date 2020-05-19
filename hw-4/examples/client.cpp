//
// Created by artem on 15.04.2020.
//

#include "Connection.hpp"
#include "utils.hpp"
#include "Errors.hpp"
#include <iostream>

int main() {
    tcp::Connection connection("0.0.0.0", 8080);
    tcp::set_nonblock(connection.get_fd());

    while (!std::cin.eof()) {
        std::string str;
        std::getline(std::cin, str);
        if (!std::cin.eof()) {
            str += "\n";
        }

        connection.write(str.data(),str.size());

        try {
            while (connection.read()) {
                std::cout << connection.get_buffer();
                connection.clear_buffer();
            }
        } catch (const tcp::socket_error &er) {
            continue;
        }
    }
    connection.write("stop", 4);

    while (true) {
        try {
            if (connection.read() == 0)
                break;
        } catch (const tcp::socket_error &er) {
            continue;
        }
        std::cout << connection.get_buffer();
        connection.clear_buffer();
    }
    return 0;
}
