#ifndef HTTPFRAMEWORK_INDEXMAKER_HPP
#define HTTPFRAMEWORK_INDEXMAKER_HPP

#include <string>
#include <ifstream>

class IndexMaker {



private:
    std::ifstream   stream_;
    std::string     filename_;
    size_t          elem_number_;
};

#endif //HTTPFRAMEWORK_INDEXMAKER_HPP
