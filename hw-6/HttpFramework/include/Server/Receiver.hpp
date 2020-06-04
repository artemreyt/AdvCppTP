#ifndef HTTPFRAMEWORK_RECEIVER_HPP
#define HTTPFRAMEWORK_RECEIVER_HPP

#include "Connection.hpp"
#include "Server.hpp"


namespace HttpFramework::Server {

    class Receiver {

        explicit    Receiver(Connection &connection);

        void        receive();


    private:

        Connection& connection_;
        std::string first_string_;
        std::string headers_;
        std::string url_params_;
        std::string body;
    };



}







#endif //HTTPFRAMEWORK_RECEIVER_HPP
