#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "Connection.hpp"
#include <utility>
#include <string>
#include <unordered_map>

namespace HttpFramework {

    namespace http_structures {

        class HttpResponse {
        public:
            explicit HttpResponse(const std::string &version, uint16_t status_code = 200);

            void setStatus(uint16_t status_code);

            bool setHeader(std::string header, std::string value);

            void setBody(std::string body);

        private:
            friend class HttpFrameworkServer;

            void send(Connection &connection);

            std::string version_;
            uint16_t status_code_;
            std::unordered_map<std::string, std::string> headers_;
            std::string body_;
        };

        class HttpResponse500 : public HttpResponse {
            explicit HttpResponse500(const std::string version);
        };

    }
}


#endif // HTTPRESPONSE_H
