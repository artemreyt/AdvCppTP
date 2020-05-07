#ifndef HTTP_ERRORS_HPP
#define HTTPP_ERRORS_HPP

#include <stdexcept>
#include <string>

namespace HttpFramework {

    class error: public std::exception {
    public:
        explicit error(std::string msg="HttpFramework error");
        const char *what() const noexcept override;

    protected:
        std::string msg_;
    };

    class tcp_error: public error {
        explicit tcp_error(std::string msg="Tcp error");
    };

    class http_error : public error {
        explicit http_error(std::string msg="Http error");
    };

    class socket_error: public tcp_error {
    public:
        explicit socket_error(std::string msg="Socket error");
    };

    class bad_ip_address: public tcp_error {
    public:
        explicit bad_ip_address(std::string ip);
        const std::string &get_ip() const;

    private:
        std::string ip_;
    };

    class connection_error: public tcp_error {
    public:
        explicit connection_error(std::string msg="Connection error");
    };

    class accept_error: public tcp_error {
    public:
        explicit accept_error(std::string msg="Accept error");
    };

    class epoll_error : public tcp_error {
    public:
        explicit epoll_error(std::string msg="epoll error");
    };

    class epollAddError: public epoll_error {
    public:
        explicit epollAddError(std::string msg="Epoll add error");
    };

    class httpNotImplemented: public http_error {
    public:
        explicit httpNotImplemented();
    };

    class badRequest: public http_error {
    public:
        explicit badRequest();
    };
}

#endif // HTTP_ERRORS_HPP
