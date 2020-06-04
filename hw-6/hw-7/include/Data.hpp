#ifndef HTTPFRAMEWORK_DATA_HPP
#define HTTPFRAMEWORK_DATA_HPP

#include <cstdint>

using Key_t = uint64_t;

struct Data {
    uint8_t payload[20];
};


#endif //HTTPFRAMEWORK_DATA_HPP
