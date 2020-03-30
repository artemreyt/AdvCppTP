#include "Logger.hpp"
#include <memory>

namespace Logger
{
    Logger::Logger():
            global_logger_(std::make_unique<StdoutLogger>())
    {}

    Logger& Logger::get_instance()
    {
        static Logger logger;
        return logger;
    }

    void Logger::set_global_logger(t_LoggerType type, const char *filename)
    {
        std::unique_ptr<BaseLogger> new_logger;
        t_level level = global_logger_->get_level();

        if (type == t_LoggerType::STDOUT_LOGGER)
            new_logger = std::make_unique<StdoutLogger>(level);
        else if (type == t_LoggerType::STDERR_LOGGER)
            new_logger = std::make_unique<StderrLogger>(level);
        else if (type == t_LoggerType::FILE_LOGGER)
        {
            if (!filename)
                return;
            new_logger = std::make_unique<FileLogger>(filename, level);
        }
        global_logger_.swap(new_logger);
    }

    BaseLogger *Logger::get_global_logger()
    {
        return global_logger_.get();
    }


    void create_file_logger(const std::string &filename)
    {
        Logger::get_instance().set_global_logger(t_LoggerType::FILE_LOGGER, filename.c_str());
    }

    void create_stdout_logger()
    {
        Logger::get_instance().set_global_logger(t_LoggerType::STDOUT_LOGGER);
    }

    void create_stderr_logger()
    {
        Logger::get_instance().set_global_logger(t_LoggerType::STDERR_LOGGER);
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