#include "HttpResponse.hpp"
#include "Coroutine.hpp"
#include "utils.hpp"
#include <string>
#include <numeric>
#include <utility>

namespace HttpFramework::http_structures {

    const std::string ENDLINE = "\r\n";

    const std::unordered_map<uint16_t, std::string> g_status_info {
            {200, "OK"},
            {400, "Bad Request"},
            {404, "Not Found"},
            {500, "Internal Server Error"},
            {501, "Not Implemented"}
    };

    HttpResponse::HttpResponse(constants::HttpVersion version, uint16_t status_code):
                version_(version), status_code_(status_code) {}

    void HttpResponse::setStatus(uint16_t status_code) {
        status_code_ = status_code;
    }

    void HttpResponse::send(Connection &connection) {
        auto &map = headers_;
        size_t sum_len = std::accumulate(headers_.begin(), headers_.end(), 0,
                    [&map](size_t first, auto &pair) {
                                return first + pair.second.size();
                            }) + 30;  // 30 - magic
        std::string msg;
        msg.reserve(sum_len);

        msg += utils::to_string(version_) + " " + std::to_string(status_code_) + " "
                + g_status_info.at(status_code_) + ENDLINE;

        for (const auto &[header_name, value] : headers_) {
            msg += (header_name + ": ").append(value).append(ENDLINE);
        }

        msg += ENDLINE + body_ + ENDLINE + ENDLINE;

        size_t sended = 0;
        while ( true ) {
            size_t bytes = connection.write(msg.data() + sended, msg.size() - sended);
            sended += bytes;

            if (sended < msg.size()) {
                Coroutine::yield();
            } else {
                break;
            }
        }
    }

    bool HttpResponse::setHeader(std::string header, std::string value) {

        auto res = headers_.emplace(std::move(header), std::move(value));
        return res.second;
    }

    void HttpResponse::setBody(std::string body) {
        body_ = std::move(body);
        setHeader("Content-Length", std::to_string(body_.size()));
    }

    HttpResponse500::HttpResponse500(constants::HttpVersion version):
            HttpResponse(std::move(version), 505) {}
}
