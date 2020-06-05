#ifndef KVREADER_UTILS_HPP
#define KVREADER_UTILS_HPP

#include "Data.hpp"

namespace KVReader::utils {
    std::size_t calculate_index_offset(Data::Key_t key);
}

#endif // KVREADER_UTILS_HPP
