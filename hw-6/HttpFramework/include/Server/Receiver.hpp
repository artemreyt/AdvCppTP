#ifndef HTTPFRAMEWORK_RECEIVER_HPP
#define HTTPFRAMEWORK_RECEIVER_HPP

#include "Connection.hpp"
#include "Server.hpp"
#include "HttpRequest.hpp"


namespace HttpFramework::Server {

    class Receiver {

    public:
        explicit    Receiver(Connection &connection);

        http_structures::HttpRequest    receive();

    private:
        void           split_until_body(std::string_view &first_line, std::string_view &headers);
        std::string    read_body(size_t start_pos, size_t body_len);

        Connection& connection_;
    };



}







#endif //HTTPFRAMEWORK_RECEIVER_HPP
