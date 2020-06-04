#ifndef HTTPFRAMEWORK_HEADERSMAP_HPP
#define HTTPFRAMEWORK_HEADERSMAP_HPP

#include <unordered_map>
#include <string>
#include <string_view>

namespace HttpFramework::http_structures {

    class HeadersMap {
    public:
        using DataType = std::unordered_map<std::string, std::string>;
        using const_iterator = DataType::const_iterator;

        explicit HeadersMap(std::string_view headers_view);

        const std::string &get(const std::string &header) const ;

        std::string to_string() const ;

        const_iterator cbegin() const ;

        const_iterator cend() const ;

        bool find(const std::string &header) const;

    private:

        std::unordered_map<std::string, std::string> data_;
        std::string string_image;
    };
}




#endif //HTTPFRAMEWORK_HEADERSMAP_HPP
