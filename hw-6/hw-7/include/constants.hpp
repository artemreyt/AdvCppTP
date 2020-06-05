#ifndef KVREADER_CONSTANTS_HPP
#define KVREADER_CONSTANTS_HPP

#include "Data.hpp"

namespace constants {


    namespace ints {
        const size_t gigabyte = 1 << 30;

        const size_t default_log_period = 100000;

        const size_t max_mmap_window_size = gigabyte / 4;
    }

    namespace types {
        using offset_t = uint64_t;
        using index_data_t = std::pair<Data::Key_t, offset_t>;
        using kv_data_t = std::pair<Data::Key_t, Data::Data>;
    }

}

#endif //KVREADER_CONSTANTS_HPP
