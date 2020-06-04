#include "HeaderMap.hpp"
#include "constants.hpp"
#include "utils.hpp"

namespace HttpFramework::http_structures {


    HeaderMap::HeaderMap(std::string_view headers_view) {
        while (!headers_view.empty()) {
            size_t linesize = headers_view.find(constants::strings::endline);
            std::string_view line = headers_view.substr(0, linesize);

            size_t header_size = line.find(constants::strings::headers::separator);
            std::string header = utils::to_lowercase(line.substr(0, header_size));




            headers_view.remove_prefix(linesize + std::string_view(constants::strings::endline).size());
        }
    }

    const std::string &HeaderMap::get(const std::string &header) const {
        return ;
    }

    std::string HeaderMap::to_string() const {
        return std::__cxx11::string();
    }

    HeaderMap::const_iterator HeaderMap::cbegin() const {
        return data_.cbegin();
    }

    HeaderMap::const_iterator HeaderMap::cend() const {
        return data_.cend();
    }
}