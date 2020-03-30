#include "Logger.hpp"
#include <vector>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <algorithm>
#include <stdexcept>


/*
 *  Default log level is INFO, to change it:
 *  -v --verbose: DEBUG level
 *  -q --quiet: ERROR level
 *  -w --warn:  WARN LEVEL
 *
 *  Default logger type is StdoutLogger, to change:
 *  --log-to-file=filename: FileLogger
 *  --stderr: StderrLogger
*/

static void set_logger(int argc, char **argv);
static void print_info();

int main(int argc, char **argv)
{
    set_logger(argc, argv);
    Logger::info("Process " + std::to_string(getpid()) + " started");

    size_t size;
    std::cout << "Enter size of vector to be created: ";
    std::cin >> size;

    std::vector<int> vec(size, 0);
    Logger::debug("Vector with size " + std::to_string(vec.size()) + " was created");

    int cmd;
    while ( true )
    {
        print_info();
        std::cin >> cmd;

        if (cmd == 1 || cmd == 2)
        {
            int index;
            std::cout << "Enter index: ";
            std::cin >> index;

            bool catched = false;
            try
            {
                if (cmd == 1)
                    std::cin >> vec.at(index);
                else
                    std::cout << vec.at(index) << std::endl;
            } catch (const std::out_of_range &err)
            {
                Logger::error("Index " + std::to_string(index) + " out of range");
                catched = true;
            }
            if (!catched && cmd == 1)
                Logger::info("Element " + std::to_string(index) + " was changed");
            else if (!catched)
                Logger::debug("Element " + std::to_string(index) + " was requested");
        }
        else if (cmd == 3)
        {
            std::for_each(vec.begin(), vec.end(), [](int& el) {std::cout << el << ' ';});
            std::cout << std::endl;
            Logger::debug("Vector print was requested");
        }
        else
        {
            Logger::warn("User entered unknown command");
            break;
        }
    }

    Logger::info("Process " + std::to_string(getpid()) + " ended");

}

void print_info()
{
    std::cout << "------------------" << std::endl;
    std::cout << "1 - change element" << std::endl;
    std::cout << "2 - get element" << std::endl;
    std::cout << "3 - print vector" << std::endl;
    std::cout << "------------------" << std::endl;
}


static const char *optstring = "vqw";

static struct option long_options[] = {
        { "verbose", 0, 0, 'v' },
        { "quiet", 0, 0, 'q' },
        { "warn", 0, 0, 'w'},
        { "log-to-file", 1, 0, 0},
        { "stderr", 0, 0, 0},
        { 0, 0, 0, 0 }
};

void set_logger(int argc, char **argv)
{
    char c;
    int index;

    while ((c = getopt_long(argc, argv, optstring, long_options, &index)) != -1)
    {
        switch (c)
        {
            case 'v':
            {
                Logger::set_level(Logger::t_level::DEBUG);
                break;
            }
            case 'q':
            {
                Logger::set_level(Logger::t_level::ERROR);
                break;
            }
            case 'w':
            {
                Logger::set_level(Logger::t_level::WARNING);
                break;
            }
            case 0:
            {
                if (long_options[index].name == std::string("log-to-file"))
                    Logger::Logger::set_global_logger(Logger::create_file_logger(optarg));
                else if (long_options[index].name == std::string("stderr"))
                    Logger::Logger::set_global_logger(Logger::create_stderr_logger());
                break;
            }
            default:
                break;
        }
    }
}