#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <cstddef>
#include "Descriptor.hpp"

namespace Process
{
    class Process
    {
    public:
        explicit Process(const std::string &path);
        ~Process();
        size_t write(const void *data, size_t len);
        void writeExact(const void *data, size_t len);
        size_t read(void *data, size_t len);
        void readExact(void *data, size_t len);

        bool isReadable() const;
        void closeStdin();
        void close();

        int getStdin() const;
        int getStdout() const;

    private:
        pid_t child_pid_;

        Descriptor child_stdin_;
        Descriptor child_stdout_;
    };
}

#endif //PROCESS_H
