#ifndef TCP_ERRORS_HPP
#define TCP_ERRORS_HPP

#include <stdexcept>
#include <string>

namespace tcp {

    class error: public std::exception {
    public:
        error(const std::string &msg);
        virtual const char *what() const noexcept override;

    protected:
        std::string msg_;
    };

    class socket_error: public error {
    public:
        socket_error(const std::string &msg="Socket error");
    };

    class bad_ip_address: public socket_error {
    public:
        bad_ip_address(const std::string& ip);
        virtual const char *what() const noexcept override;
        const std::string &get_ip() const;

    private:
        std::string ip_;
    };
}


#endif //TCP_ERRORS_HPP
