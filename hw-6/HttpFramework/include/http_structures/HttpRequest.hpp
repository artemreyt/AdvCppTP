#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "Connection.hpp"
#include <unordered_map>
#include <string>

namespace HttpFramework {
    namespace http_structures {

        class HttpRequest {
        public:
            class FirstLine;

            using headers_type = std::unordered_map<std::string, std::string>;

            enum class methods {
                GET, POST, UNSUPPORTED
            };

            explicit HttpRequest(Connection &con);

            void receive_request();

            void read_get();

            void read_post(size_t start);

            const headers_type &get_headers() const;

            const headers_type &get_params() const;

            const methods &get_method() const;

            const std::string &get_path() const;

            const std::string &get_version() const;

            const std::string &get_body() const;

        private:

            void parse_headers(size_t end = 0);

            Connection &connection_;
            headers_type headers_;
            headers_type params_;
            methods method_;
            std::string path_;
            std::string version_;
            std::string body_;
        };

    }
}


#endif // HTTPREQUEST_HPP
