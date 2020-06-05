#ifndef HTTPFRAMEWORK_RANDOMREADER_HPP
#define HTTPFRAMEWORK_RANDOMREADER_HPP


#include "Descriptor.hpp"
#include <string>

namespace KVReader::MmapIO {

    class RandomReader {
    public:
        explicit RandomReader(std::string filepath);


        template<typename T>
        void get(T &data, size_t offset);


    private:

        Descriptor::Descriptor fd_;
        const std::string filepath_;
        const size_t filesize_;
    };
}


#endif //HTTPFRAMEWORK_RANDOMREADER_HPP
