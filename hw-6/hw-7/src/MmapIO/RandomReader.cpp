#include "RandomReader.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


namespace KVReader::MmapIO {

    RandomReader::RandomReader(std::string filepath):
            fd_(::open(filepath.c_str(), O_RDONLY)), filepath_(std::move(filepath)){

        if (fd_ == -1) {
            throw std::runtime_error("Can't open index file");
        }
    }

}