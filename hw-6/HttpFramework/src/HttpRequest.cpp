#include "HttpRequest.hpp"
#include "Coroutine.hpp"
#include "Errors.hpp"
#include "utils.hpp"
#include <sstream>


namespace HttpFramework {
    HttpRequest::HttpRequest(Connection &con): connection_(con) {}

    void HttpRequest::receive_request() {
        auto &buffer = connection_.get_buffer();
        size_t pos = 0;
        size_t headers_end;

        while (true) {
            size_t bytes = connection_.read();

            headers_end= buffer.find("\r\n\r\n", pos);
            if (headers_end != std::string::npos) {
                parse_headers(headers_end);
                break;
            }
            pos += bytes;
            Coroutine::yield();
        }

        if (method_ == "POST") {
            read_post(headers_end + 4);       // 4 = strlen("\r\n\r\n")
        } else if (method_ == "GET") {
            read_get();
        } else {
            throw httpNotImplemented("Only GET and POST methods are available");
        }
    }

    void HttpRequest::parse_headers(size_t end) {
        auto &buffer = connection_.get_buffer();
        std::istringstream stream(buffer.substr(0, end ? end: std::string::npos));

        stream >> method_ >> path_ >> version_;
        version_ = version_.substr(version_.find('/') + 1);
        stream.ignore(2);

        while (!stream.eof()) {
            std::string line;
            std::getline(stream, line, '\r');
            if (line.empty()) {
                break;
            }
            stream.ignore(2);

            size_t delim_pos = line.find(':');
            std::string name = line.substr(0, delim_pos);
            std::string value = line.substr(delim_pos + 1);
            lstrip(value);
            headers_.emplace(std::move(name), std::move(value));
        }
    }

    void HttpRequest::read_get() {
        if (path_.find("/?") != std::string::npos) {
            std::string query_string = path_.substr(path_.find("/?") + 2);
            parse_query_string(query_string, params_);
        }
    }

    void HttpRequest::read_post(size_t start) {
        auto &buffer = connection_.get_buffer();
        auto content_type = headers_.at("Content-Type");
        auto content_len = std::stoll(headers_.at("Content-Length"));

        while (content_len > 0) {
            size_t bytes = connection_.read();
            content_len -= bytes;
            Coroutine::yield();
        }
        body_ = buffer.substr(start);

        if (content_type.find("application/x-www-form-urlencoded") != std::string::npos)
            parse_query_string(body_, params_);
    }

    /*-------------------Getters--------------------------*/

    const HttpRequest::headers_type  &HttpRequest::get_headers() const {
        return headers_;
    }

    const HttpRequest::headers_type  &HttpRequest::get_params() const {
        return params_;
    }

    const std::string &HttpRequest::get_method() const {
        return method_;
    }

    const std::string &HttpRequest::get_path() const {
        return path_;
    }

    const std::string &HttpRequest::get_version() const {
        return version_;
    }

    const std::string &HttpRequest::get_body() const {
        return body_;
    }
}
