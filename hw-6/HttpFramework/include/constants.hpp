#ifndef HTTPFRAMEWORK_CONSTANTS_HPP
#define HTTPFRAMEWORK_CONSTANTS_HPP

namespace HttpFramework::constants {

    namespace strings {

        const char *endline = "\r\n";
        const char *percent = "%";
        const char *question_mark = "?";
        const char *whitespaces = " \t";
        const char *headers_end = "\r\n\r\n";

        namespace urlencoded {
            const char *pair_separator = "&";
            const char *kv_separator = "=";
        }

        namespace headers {
            const char *separator = ":";
            const char *content_length = "content-length";

        }
    }
}


#endif //HTTPFRAMEWORK_CONSTANTS_HPP
