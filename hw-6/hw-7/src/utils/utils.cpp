#include "utils.hpp"
#include "constants.hpp"


namespace KVReader::utils {

    std::size_t calculate_index_offset(Data::Key_t key) {
        return (key - 1) * sizeof(constants::types::index_data_t);
    }



}