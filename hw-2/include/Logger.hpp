#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "BaseLogger.hpp"
#include <memory>

namespace Logger
{
    enum class t_LoggerType
    {
        STDOUT_LOGGER,
        STDERR_LOGGER,
        FILE_LOGGER
    };

    class Logger
    {
    public:
        static Logger& get_instance();
        bool set_global_logger(t_LoggerType type, const char *filename=nullptr);
        BaseLogger *get_global_logger();

        Logger(const Logger &logger) = delete;
        Logger& operator=(const Logger &rhs) = delete;

    private:
        Logger();

        std::unique_ptr<BaseLogger> global_logger_;
    };


    void create_file_logger(const std::string &filename);
    void create_stdout_logger();
    void create_stderr_logger();

    void debug(const std::string &msg);
    void info(const std::string &msg);
    void warn(const std::string &msg);
    void error(const std::string &msg);
    void set_level(t_level level);
}


#endif // LOGGER_HPP
