#ifndef HTTPFRAMEWORK_RANDOMWRITER_HPP
#define HTTPFRAMEWORK_RANDOMWRITER_HPP

#include "Descriptor.hpp"
#include <unordered_map>
#include <unistd.h>
#include <sys/mman.h>

namespace KVReader::MmapIO {

    class RandomWriter {
    public:
        explicit RandomWriter(std::string filepath);


        template <typename T>
        void put(const T& data, size_t offset);



    private:

        Descriptor::Descriptor  fd_;
        const std::string       filepath_;

        size_t                  filesize_=0;
    };

    template <typename T>
    void RandomWriter::put(const T& data, size_t offset) {
        if (offset >= filesize_) {
            filesize_ += sizeof(data);

            if (::ftruncate(fd_, filesize_) == -1) {
                throw std::runtime_error("Truncate error: ");
            }
        }

        char *memmap = static_cast<char *>(::mmap(nullptr, sizeof(data), PROT_WRITE, MAP_SHARED, fd_, offset));

        if (memmap == MAP_FAILED) {
            throw std::runtime_error("mmap fail in RandomWriter::put()");
        }
    }
}

#endif //HTTPFRAMEWORK_RANDOMWRITER_HPP
