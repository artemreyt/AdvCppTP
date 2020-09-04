#ifndef HTTPFRAMEWORK_FIRSTLINES_H
#define HTTPFRAMEWORK_FIRSTLINES_H

#include "constants.hpp"
#include <string_view>
#include <string>

namespace HttpFramework::http_structures {

    class QueryLine {
    public:
        explicit QueryLine(std::string_view line);
        QueryLine(QueryLine &&other) = default;

        std::string to_string();

        const std::string           &get_path() const;
        constants::HttpVersion      get_version() const;
        constants::RequestMethod    get_method() const;


    private:
        constants::RequestMethod    method_=constants::RequestMethod::UNSUPPORTED;
        constants::HttpVersion      version_=constants::HttpVersion::UNSUPPORTED;
        std::string                 path_;
    };

}


#endif //HTTPFRAMEWORK_FIRSTLINES_H
