#include "FirstLines.h"
#include "utils.hpp"
#include "constants.hpp"
#include <sstream>

namespace HttpFramework::http_structures {

    QueryLine::QueryLine(std::string_view line) {
        std::stringstream sstream((std::string(line)));
        std::string method, version;
        sstream >> method >> path_ >> version;
        method = utils::to_lowercase(method);

        if (method == "get") {
            method_ = constants::RequestMethod::GET;
        } else if (method == "post") {
            method_ = constants::RequestMethod::POST;
        }

        std::string_view version_view = version;
        version_view.remove_prefix(version_view.find(constants::strings::version_sep) + 1);
        if (version_view == "1.0") {
            version_ = constants::HttpVersion::V1_0;
        } else if (version_view == "1.1") {
            version_ = constants::HttpVersion::V1_1;
        }
    }

    std::string QueryLine::to_string() {
        return (utils::to_string(method_) + " ").append(path_ + " ").append(utils::to_string(version_));
    }

    const std::string &QueryLine::get_path() const {
        return path_;
    }

    constants::HttpVersion QueryLine::get_version() const {
        return version_;
    }

    constants::RequestMethod QueryLine::get_method() const {
        return method_;
    }
}