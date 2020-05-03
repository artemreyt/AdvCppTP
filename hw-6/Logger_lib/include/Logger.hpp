#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "BaseLogger.hpp"
#include <memory>

namespace Logger
{
    class Logger
    {
    public:
        static Logger& get_instance();
        static void set_global_logger(std::unique_ptr<BaseLogger> logger);
        static BaseLogger *get_global_logger();

        Logger(const Logger &logger) = delete;
        Logger& operator=(const Logger &rhs) = delete;

    private:
        Logger();

        std::unique_ptr<BaseLogger> global_logger_;
    };


    std::unique_ptr<FileLogger> create_file_logger(const std::string &filename);
    std::unique_ptr<StdoutLogger> create_stdout_logger();
    std::unique_ptr<StderrLogger> create_stderr_logger();

    void debug(const std::string &msg);
    void info(const std::string &msg);
    void warn(const std::string &msg);
    void error(const std::string &msg);
    void set_level(t_level level);
}


#endif // LOGGER_HPP
