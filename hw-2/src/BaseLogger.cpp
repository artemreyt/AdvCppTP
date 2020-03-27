#include "BaseLogger.hpp"
#include "LoggerErrors.hpp"
#include <sstream>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string_view>

namespace Logger
{
    BaseLogger::BaseLogger(std::ostream &stream, t_level level):
        stream_(stream), level_(level)
    {}

    void BaseLogger::debug(const std::string &msg)
    {
        if (level_ == t_level::DEBUG)
            log(msg, t_level::DEBUG);
    }

    void BaseLogger::info(const std::string &msg)
    {
        if (level_ >= t_level::INFO)
            log(msg, t_level::INFO);
    }

    void BaseLogger::warn(const std::string &msg)
    {
        if (level_ >= t_level::WARNING)
            log(msg, t_level::WARNING);
    }

    void BaseLogger::error(const std::string &msg)
    {
        log(msg, t_level::ERROR);
    }

    void BaseLogger::log(const std::string &msg, t_level level)
    {
        std::ostringstream note;
        try
        {
            if (level == t_level::DEBUG)
                note << "DEBUG";
            else if (level == t_level::INFO)
                note << "INFO";
            else if (level == t_level::WARNING)
                note << "WARNING";
            else if (level == t_level::ERROR)
                note << "ERROR";
            else
                throw UnknownLevelError(level);

            std::time_t cur_time = std::time(nullptr);
            if (cur_time != -1)
            {
                std::string_view time_str = std::ctime(&cur_time);
                time_str.remove_suffix(1);
                note << "(" << time_str << ")";
            }

            note << ": << " << msg << " >>";
        } catch (std::ios_base::failure&)
        {
            throw LogRuntimeError("Fail to create to log message");
        }

        try
        {
            stream_ << note.str() << std::endl;
        } catch (std::ios_base::failure&)
        {
            throw LogRuntimeError("Fail to write to log stream");
        }
    }

    void BaseLogger::set_level(t_level level)
    {
        if (level > t_level::DEBUG)
            throw UnknownLevelError(level);
        level_ = level;
    }

    t_level BaseLogger::get_level() const
    {
        return level_;
    }

    void BaseLogger::flush()
    {
        stream_.flush();
    }

    StdoutLogger::StdoutLogger(t_level level):
        BaseLogger(std::cout, level)
    {}

    StderrLogger::StderrLogger(t_level level):
        BaseLogger(std::cerr, level)
    {}

    FileLogger::FileLogger(const std::string &path, t_level level):
        outfile(path), BaseLogger(outfile, level)
    {}
}