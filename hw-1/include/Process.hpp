#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <cstddef>
#include <vector>

class Process
{
public:
    explicit Process(const std::string& path);
    ~Process();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool isReadable() const;
    void closeStdin();

    void close();

private:

    pid_t	cpid;
    int		child_stdin;
    int		child_stdout;
    const std::vector<char *> params;
};

#endif //PROCESS_H
