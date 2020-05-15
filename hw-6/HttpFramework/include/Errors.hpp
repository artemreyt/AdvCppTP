#ifndef HTTP_ERRORS_HPP
#define HTTP_ERRORS_HPP

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

    /*------------Base Errors--------------*/

    class tcp_error: public error {
    public:
        explicit tcp_error(std::string msg="Tcp error");
    };

    class http_error : public error {
    public:
        explicit http_error(std::string msg="Http error");
    };

    class epoll_error : public error {
    public:
        explicit epoll_error(std::string msg="epoll error");
    };

    /*------------Tcp Errors----------------*/

    class socket_creation_error: public tcp_error {
    public:
        explicit socket_creation_error(std::string="Socket creation error");
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

    /*-------------Epoll Errors----------------*/

    class epollAddError: public epoll_error {
    public:
        explicit epollAddError(std::string msg="Epoll add error");
    };

    /*-------------Http Errors------------------*/

    class httpNotImplemented: public http_error {
    public:
        explicit httpNotImplemented(std::string msg="501 Not Implemented");
    };

    class httpBadRequest: public http_error {
    public:
        explicit httpBadRequest(std::string msg="400 Bad Request");
    };
}

#endif // HTTP_ERRORS_HPP
