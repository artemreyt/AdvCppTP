#ifndef BASELOGGER_HPP
#define BASELOGGER_HPP

#include <string>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <fstream>

namespace Logger
{
    enum class t_level : unsigned int
    {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    class BaseLogger
    {
    public:
        BaseLogger(std::ostream *stream, t_level level);
        BaseLogger(BaseLogger &&other) noexcept ;

        BaseLogger &operator=(const BaseLogger& other) noexcept = default;
        BaseLogger &operator=(BaseLogger &&other) noexcept ;

        void debug(const std::string &msg);
        void info(const std::string &msg);
        void warn(const std::string &msg);
        void error(const std::string &msg);
        void set_level(t_level level);
        t_level get_level() const;
        virtual void flush();

    protected:
        t_level level_;
        std::ostream *stream_;

    private:
        virtual void log(const std::string &msg, t_level level);
    };


    class StdoutLogger : public BaseLogger
    {
    public:
        explicit StdoutLogger(t_level level = t_level::INFO);
        StdoutLogger(StdoutLogger &&other) noexcept ;

        StdoutLogger &operator=(const StdoutLogger &other) noexcept = default;
        StdoutLogger &operator=(StdoutLogger &&other) noexcept = default;
    };


    class StderrLogger : public BaseLogger
    {
    public:
        explicit StderrLogger(t_level level = t_level::INFO);
        StderrLogger(StderrLogger &&other) noexcept ;

        StderrLogger &operator=(const StderrLogger &other) noexcept = default;
        StderrLogger &operator=(StderrLogger &&other) noexcept = default;
    };


    class FileLogger : public BaseLogger
    {
    public:
        explicit FileLogger(const std::string &path, t_level level = t_level::INFO);
        FileLogger(FileLogger &&other) noexcept;

        FileLogger &operator=(const FileLogger &other) = delete ;
        FileLogger &operator=(FileLogger &&other) noexcept ;

    private:
        std::ofstream outfile_;
    };
}

#endif // BASELOGGER_HPP
