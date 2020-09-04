#ifndef HTTPFRAMEWORK_INDEXMAKER_HPP
#define HTTPFRAMEWORK_INDEXMAKER_HPP

#include "Logger.hpp"
#include "constants.hpp"
#include <string>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;


namespace KVReader {
    class IndexMaker {
    public:

        IndexMaker(fs::path indexfile_path, fs::path datafile_path, Logger::BaseLogger &logger, size_t log_period);

        void make();


    private:
        void removeIndex();

        Logger::BaseLogger  &logger_;
        fs::path            datafile_path_;
        fs::path            indexfile_path_;
        size_t              log_period_ = constants::ints::default_log_period;
    };
}

#endif //HTTPFRAMEWORK_INDEXMAKER_HPP
