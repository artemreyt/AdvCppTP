#ifndef HTTPFRAMEWORK_FORWARDREADER_HPP
#define HTTPFRAMEWORK_FORWARDREADER_HPP

#include "Descriptor.hpp"
#include <string>
#include <cstring>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

namespace KVReader::MmapIO {

    class ForwardReader {

    public:
        ForwardReader(std::string filepath_, size_t interval);

        template <typename T>
        void get(T& data);

        size_t get_offset() const;

        bool eof() const;


    private:

        void    get_new_window();

        Descriptor::Descriptor  fd_;
        const std::string       filepath_;
        const size_t            filesize_;
        const size_t            interval_;


        size_t  main_offset_=0;

        struct WindowInfo {
            char    *addr = nullptr;
            size_t  offset = 0;
            size_t  window_size = 0;
        } windowInfo_;
    };



    template <typename T>
    void ForwardReader::get(T& data) {
        if (sizeof(data) > interval_) {
            throw std::invalid_argument("sizeof data bigger than interval");
        }

        if (windowInfo_.offset >= windowInfo_.window_size) {
            get_new_window();
        }

        std::memcpy(&data, windowInfo_.addr + windowInfo_.offset, sizeof(data));

        windowInfo_.offset += interval_;
        main_offset_ += interval_;
    }
}

#endif //HTTPFRAMEWORK_FORWARDREADER_HPP
