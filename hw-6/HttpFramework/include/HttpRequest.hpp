#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "Connection.hpp"
#include <map>
#include <string>

namespace HttpFramework {

    class HttpRequest {
        using headers_type = std::map<std::string, std::string>;

    public:
        explicit    HttpRequest(Connection &con);
        void        receive_request();
        void        read_get();
        void        read_post(size_t start);

        const headers_type &get_headers();
        const headers_type &get_params();
        const std::string  &get_method();
        const std::string  &get_path();
        const std::string  &get_version();
        const std::string  &get_body();

    private:

        void parse_headers(size_t end=0);

        Connection &connection_;
        std::map<std::string, std::string> headers_;
        std::map<std::string, std::string> params_;
        std::string method_;
        std::string path_;
        std::string version_;
        std::string body_;
    };
}


#endif // HTTPREQUEST_HPP
