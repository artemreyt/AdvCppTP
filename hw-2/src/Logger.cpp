#include "Logger.hpp"
#include <memory>

namespace Logger
{
    Logger::Logger():
            global_logger_(create_stdout_logger())
    {}

    Logger& Logger::get_instance()
    {
        static Logger logger;
        return logger;
    }

    void Logger::set_global_logger(std::unique_ptr<BaseLogger> new_logger)
    {
        get_instance().global_logger_.swap(new_logger);
    }

    std::unique_ptr<BaseLogger> &Logger::get_global_logger()
    {
        return get_instance().global_logger_;
    }


    std::unique_ptr<FileLogger> create_file_logger(const std::string &filename)
    {
        return std::make_unique<FileLogger>(filename);
    }

    std::unique_ptr<StdoutLogger> create_stdout_logger()
    {
        return std::make_unique<StdoutLogger>();
    }

    std::unique_ptr<StderrLogger> create_stderr_logger()
    {
        return std::make_unique<StderrLogger>();
    }

    void debug(const std::string &msg)
    {
        Logger::get_instance().get_global_logger()->debug(msg);
    }

    void info(const std::string &msg)
    {
        Logger::get_instance().get_global_logger()->info(msg);
    }

    void warn(const std::string &msg)
    {
        Logger::get_instance().get_global_logger()->warn(msg);
    }

    void error(const std::string &msg)
    {
        Logger::get_instance().get_global_logger()->error(msg);
    }

    void set_level(t_level level)
    {
        Logger::get_instance().get_global_logger()->set_level(level);
    }
}