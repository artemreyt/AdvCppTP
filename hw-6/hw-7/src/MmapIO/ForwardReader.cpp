#include "ForwardReader.hpp"
#include "constants.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <cerrno>
#include <string>

using std::literals::string_literals::operator""s;

namespace KVReader::MmapIO {

    ForwardReader::ForwardReader(std::string filepath, size_t interval):
            filesize_(fs::file_size(filepath)), fd_(::open(filepath.c_str(), O_RDONLY)),
            interval_(interval), filepath_(std::move(filepath)) {

        if (fd_ == -1) {
            throw std::exception(); // TODO: exception
        }

        get_new_window();
    }


    size_t ForwardReader::get_offset() const {
        return main_offset_;
    }

    void ForwardReader::get_new_window() {
        if (windowInfo_.addr) {
            munmap(windowInfo_.addr, windowInfo_.window_size);
        }

        size_t tail_size = filesize_ - main_offset_;

        if (tail_size > constants::ints::max_mmap_window_size) {
            windowInfo_.window_size = constants::ints::max_mmap_window_size;
        } else {
            windowInfo_.window_size = tail_size;
        }

        windowInfo_.window_size = (windowInfo_.window_size / interval_) * interval_; // window_size % interval = 0

        windowInfo_.addr = static_cast<char *>(::mmap(nullptr, windowInfo_.window_size,
                                                      PROT_READ, MAP_PRIVATE, fd_, main_offset_));

        if (windowInfo_.addr == MAP_FAILED) {
            throw std::runtime_error("get_new_window (Map failed): "s.append(std::strerror(errno)));
        }

        windowInfo_.offset = 0;
    }

    bool ForwardReader::eof() const {
        return main_offset_ >= filesize_;
    }

}