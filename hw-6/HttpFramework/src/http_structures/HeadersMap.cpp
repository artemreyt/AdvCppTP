#include "HeadersMap.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include <cstring>

namespace HttpFramework::http_structures {


    HeadersMap::HeadersMap(std::string_view headers_view) {
        while (!headers_view.empty()) {
            size_t linesize = headers_view.find(constants::strings::endline);
            std::string_view line = headers_view.substr(0, linesize);

            size_t header_size = line.find(constants::strings::headers::separator);

            std::string_view header_view = line.substr(0, header_size);
            std::string header = utils::to_lowercase(utils::rtrim(header_view));

            header_size += std::string_view(constants::strings::headers::separator).size();
            size_t value_pos = line.find_first_not_of(constants::strings::whitespaces, header_size);
            std::string value = std::string(line.substr(value_pos));

            data_.emplace(std::move(header), std::move(value));

            headers_view.remove_prefix(linesize + ::strlen(constants::strings::endline));
        }
    }

    const std::string &HeadersMap::get(const std::string &header) const {
        return data_.at(header);
    }

    std::string HeadersMap::to_string() const {
        std::string result;

        for (const auto &[header, value]: data_) {
            result += (header + constants::strings::headers::separator).append(" ").append(value)
                        .append(constants::strings::endline);
        }
        return result;
    }

    HeadersMap::const_iterator HeadersMap::cbegin() const {
        return data_.cbegin();
    }

    HeadersMap::const_iterator HeadersMap::cend() const {
        return data_.cend();
    }

    bool HeadersMap::find(const std::string &header) const {
        return data_.find(header) != data_.end();
    }
}