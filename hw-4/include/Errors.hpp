#ifndef TCP_ERRORS_HPP
#define TCP_ERRORS_HPP

#include <stdexcept>
#include <string>

namespace tcp {

    class error: public std::exception {
    public:
        explicit error(std::string msg="tcp::error");
        const char *what() const noexcept override;

    protected:
        std::string msg_;
    };

    class socket_error: public error {
    public:
        explicit socket_error(std::string msg="Socket error");
    };

    class bad_ip_address: public socket_error {
    public:
        explicit bad_ip_address(std::string ip);
        const std::string &get_ip() const;

    private:
        std::string ip_;
    };

    class connection_error: public error {
    public:
        explicit connection_error(std::string msg="Connection error");
    };

    class accept_error: public error {
    public:
        explicit accept_error(std::string msg="Accept error");
    };

    class epoll_error : public error {
    public:
        explicit epoll_error(std::string msg="epoll error");
    };

    class epollAddError: public error {
    public:
        explicit epollAddError(std::string msg="Epoll add error");
    };

}


#endif //TCP_ERRORS_HPP
