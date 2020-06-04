#ifndef HTTPFRAMEWORK_HEADERMAP_HPP
#define HTTPFRAMEWORK_HEADERMAP_HPP

#include <unordered_map>
#include <string>
#include <string_view>

namespace HttpFramework::http_structures {

    class HeaderMap {
        using DataType = std::unordered_map<std::string, std::string>;
        using const_iterator = DataType::const_iterator;

        explicit HeaderMap(std::string_view headers_view);

        const std::string &get(const std::string &header) const ;

        std::string to_string() const ;

        const_iterator cbegin() const ;

        const_iterator cend() const ;



    private:

        std::unordered_map<std::string, std::string> data_;
        std::string string_image;
    };
}




#endif //HTTPFRAMEWORK_HEADERMAP_HPP
