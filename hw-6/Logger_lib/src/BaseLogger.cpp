#include "BaseLogger.hpp"
#include "LoggerErrors.hpp"
#include <sstream>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string_view>
#include <map>

namespace Logger
{
    static const std::map<t_level, std::string> g_level_invert =
    {
            {t_level::DEBUG,   "DEBUG"},
            {t_level::INFO,    "INFO"},
            {t_level::WARNING, "WARN"},
            {t_level::ERROR,   "ERROR"}
    };

    BaseLogger::BaseLogger(std::ostream *stream, t_level level):
        stream_(stream), level_(level)
    {}

    BaseLogger::BaseLogger(BaseLogger &&other) noexcept :
        stream_(other.stream_), level_(other.level_)
    {
        other.stream_ = nullptr;
    }

    BaseLogger &BaseLogger::operator=(BaseLogger &&other) noexcept
    {
        stream_ = other.stream_;
        level_ = other.level_;
        other.stream_ = nullptr;
        return *this;
    }

    void BaseLogger::debug(const std::string &msg)
    {
        if (level_ >= t_level::DEBUG)
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
        if (level_ >= t_level::ERROR)
            log(msg, t_level::ERROR);
    }

    void BaseLogger::log(const std::string &msg, t_level level)
    {
        std::ostringstream note;
        try
        {
            note << g_level_invert.at(level);

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
            *stream_ << note.str() << std::endl;
        } catch (std::ios_base::failure&)
        {
            throw LogRuntimeError("Fail to write to log stream");
        }
    }

    void BaseLogger::set_level(t_level level)
    {
        level_ = level;
    }

    t_level BaseLogger::get_level() const
    {
        return level_;
    }

    void BaseLogger::flush()
    {
        stream_->flush();
    }

    StdoutLogger::StdoutLogger(t_level level):
        BaseLogger(&std::cout, level)
    {}

    StdoutLogger::StdoutLogger(StdoutLogger &&other) noexcept :
        BaseLogger(std::move(other))
    {}


    StderrLogger::StderrLogger(t_level level):
        BaseLogger(&std::cerr, level)
    {}

    StderrLogger::StderrLogger(StderrLogger &&other) noexcept:
        BaseLogger(std::move(other))
    {}

    FileLogger::FileLogger(const std::string &path, t_level level):
        outfile_(path), BaseLogger(&outfile_, level)
    {}

    FileLogger::FileLogger(Logger::FileLogger &&other) noexcept:
        outfile_(std::move(other.outfile_)), BaseLogger(std::move(other))
    {}

    FileLogger &FileLogger::operator=(FileLogger &&other) noexcept
    {
        stream_ = other.stream_;
        outfile_ = std::move(other.outfile_);
        level_ = other.level_;
        other.stream_ = nullptr;
        return *this;
    }
}