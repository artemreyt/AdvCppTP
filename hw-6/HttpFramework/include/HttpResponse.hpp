#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "Connection.hpp"
#include <utility>
#include <string>
#include <map>

namespace HttpFramework {

    class HttpResponse {
    public:
        explicit HttpResponse(const std::string &version, uint16_t status_code=200);
        void setStatus(uint16_t status_code);

        template <typename ...Args>
        bool setHeader(Args&&... args) {
            auto res = headers_.emplace(std::forward<Args>(args)...);
            return res.second;
        }

        template <typename Str>
        std::enable_if_t<std::is_assignable_v<std::string, Str>, void>
        setBody(Str &&body) {
            body_ = std::forward<Str>(body);
        }

    private:
        friend class Server;
        void send(Connection &connection);

        std::string version_;
        uint16_t    status_code_;
        std::map<std::string, std::string> headers_;
        std::string body_;
    };
}


#endif // HTTPRESPONSE_H
