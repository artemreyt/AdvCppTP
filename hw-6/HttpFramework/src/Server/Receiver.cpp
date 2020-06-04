#include "Receiver.hpp"
#include "Connection.hpp"
#include "Coroutine.hpp"
#include "constants.hpp"
#include <string>
#include <string_view>

namespace HttpFramework::Server {

    Receiver::Receiver(HttpFramework::Connection &connection): connection_(connection) {

    }

    http_structures::HttpRequest  Receiver::receive() {

        auto &buffer = connection_.get_buffer();

        size_t pos = 0;
        size_t headers_end_pos;

        while (true) {
            size_t bytes = connection_.read();

            headers_end_pos = buffer.find(constants::strings::headers_end, pos);
            if (headers_end_pos != std::string::npos) {
                break;
            }
            pos += bytes;
            Coroutine::yield();
        }

        std::string_view first_line_view, headers_view;
        split_until_body(first_line_view, headers_view);

        http_structures::HttpRequest::FirstLine first_line(first_line_view);
        http_structures::HeadersMap headers(headers_view);

        std::string body;
        if (headers.find(constants::strings::headers::content_length)) {
            size_t start_pos = headers_end_pos + std::string_view(constants::strings::headers_end).size();
            size_t body_len = std::stoull(headers.get(constants::strings::headers::content_length));

            body = read_body(start_pos, body_len);
        }

        return http_structures::HttpRequest(first_line, headers, body);
    }


    void Receiver::split_until_body(std::string_view &first_line, std::string_view &headers) {
        std::string_view view = connection_.get_buffer();
        size_t size = view.find(constants::strings::endline);

        first_line_ = view.substr(0, size);
        view.remove_prefix(size);

        size = view.find(constants::strings::headers_end);
        headers_ = view.substr(0, size);

    }
}