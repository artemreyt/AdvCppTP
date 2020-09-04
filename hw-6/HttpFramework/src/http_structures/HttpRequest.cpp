#include "HttpRequest.hpp"
#include "FirstLines.h"
#include "Coroutine.hpp"
#include "utils/utils.hpp"
#include "constants.hpp"
#include <string>


namespace HttpFramework::http_structures {
    void HttpRequest::make_params() {
        const std::string &path = first_line_.get_path();
        size_t pos = path.find(constants::strings::question_mark);

        if (pos != std::string::npos) {
            utils::parse_query_string(std::string_view(path).substr(pos + 1), params_);
        }

    }

    const QueryLine &HttpRequest::get_first_line() const {
        return first_line_;
    }

    const HeadersMap &HttpRequest::get_headers() const {
        return headers_;
    }

    const HttpRequest::ParamsType &HttpRequest::get_params() const {
        return params_;
    }

    const std::string &HttpRequest::get_body() const {
        return body_;
    }
}
