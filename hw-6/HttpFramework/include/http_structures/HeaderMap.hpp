#ifndef HTTPFRAMEWORK_HEADERMAP_HPP
#define HTTPFRAMEWORK_HEADERMAP_HPP

#include <unordered_map>
#include <string>

namespace HttpFramework::http_structures {

    class HeaderMap {



    private:

        std::unordered_map<std::string, std::string> data_;
    };
}




#endif //HTTPFRAMEWORK_HEADERMAP_HPP
