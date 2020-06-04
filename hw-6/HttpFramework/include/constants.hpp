#ifndef HTTPFRAMEWORK_CONSTANTS_HPP
#define HTTPFRAMEWORK_CONSTANTS_HPP

namespace HttpFramework::constants {

    namespace strings {

        const char *endline = "\r\n";
        const char *percent = "%";
        const char *question_mark = "?";
        const char *whitespaces = " \t";
        const char *headers_end = "\r\n\r\n";
        const char *version_sep = "/";

        namespace urlencoded {
            const char *pair_separator = "&";
            const char *kv_separator = "=";
        }

        namespace headers {
            const char *separator = ":";
            const char *content_length = "content-length";

        }
    }

    enum class RequestMethod {
        GET, POST, UNSUPPORTED
    };

    enum class HttpVersion {
        V1_0, V1_1, UNSUPPORTED
    };
}


#endif //HTTPFRAMEWORK_CONSTANTS_HPP
