#ifndef HTTPFRAMEWORK_CONSTANTS_HPP
#define HTTPFRAMEWORK_CONSTANTS_HPP

namespace HttpFramework::constants {

    namespace strings {

        const char* const endline = "\r\n";
        const char* const percent = "%";
        const char* const question_mark = "?";
        const char* const whitespaces = " \t";
        const char* const headers_end = "\r\n\r\n";
        const char* const version_sep = "/";

        namespace urlencoded {
            const char* const pair_separator = "&";
            const char* const kv_separator = "=";
        }

        namespace headers {
            const char* const separator = ":";
            const char* const content_length = "content-length";

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
