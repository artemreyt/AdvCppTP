#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "Connection.hpp"
#include "HeadersMap.hpp"
#include "FirstLines.h"
#include <unordered_map>
#include <string>
#include <utility>

namespace HttpFramework::http_structures {

        class HttpRequest {
        public:
            using ParamsType = std::unordered_map<std::string, std::string>;

            template <typename LineType, typename HeadersType, typename Str>
            explicit HttpRequest(LineType&& first_line, HeadersType&& headers, Str&& body):
                first_line_(std::forward<LineType>(first_line)),
                headers_(std::forward<HeadersType>(headers)),
                body_(std::forward<Str>(body)) {
                    make_params();
            }

            const QueryLine &get_first_line() const;
            const HeadersMap &get_headers() const;
            const ParamsType &get_params() const;
            const std::string &get_body() const;

        private:
            void make_params();

            QueryLine   first_line_;
            HeadersMap  headers_;
            ParamsType params_;
            std::string body_;
    };
}


#endif // HTTPREQUEST_HPP
