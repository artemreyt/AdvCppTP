#include "IndexMaker.hpp"
#include "Data.hpp"
#include "constants.hpp"
#include "ForwardReader.hpp"
#include "RandomWriter.hpp"
#include <string_view>
#include <utility>

using std::literals::string_literals::operator""s;

namespace KVReader {


    IndexMaker::IndexMaker(fs::path indexfile_path, fs::path datafile_path, Logger::BaseLogger &logger,
                           size_t log_period) :
        indexfile_path_(std::move(indexfile_path)), datafile_path_(std::move(datafile_path)),
        logger_(logger), log_period_(log_period) {

    }

    void IndexMaker::make() {
        MmapIO::RandomWriter  writer(indexfile_path_);
        MmapIO::ForwardReader reader(datafile_path_, sizeof(constants::types::kv_data_t));

        size_t done = 0;
        size_t next_log = log_period_;
        logger_.info("Start making index file");

        while (!reader.eof()) {
            Data::Key_t key;
            constants::types::offset_t offset;

            try {
                offset = reader.get_offset();
                reader.get(key);

                auto data = std::make_pair(key, offset);
                writer.put(data, utils::calculate_index_offset(key));


                ++done;
                if (done >= next_log) {
                    logger_.info("Written "s.append(std::to_string(done)).append(" elements"));
                    next_log += log_period_;
                }

            } catch (const KVReader::errors::error &ex) {
                logger_.error(ex.what());
                removeIndex();
                throw;
            }
        }
    }
}