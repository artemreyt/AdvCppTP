#include <iostream>
#include "Process.hpp"
#include <string>
#include <string.h>
#include "Protocol.hpp"
#include <sys/wait.h>


/*
    * this process scans lines from stdin;
    * sends it to child process via protocol "MSG_SIZE|MSG";
    * child process returns sended message in UPPERCASE;
    * this process prints on STDOUT recieved message;
    * this process ends when stdin eof found;
*/

int main()
{
    Process proc("./child.out");
    std::string buffer;

    while (true)
    {
        std::getline(std::cin, buffer);
        if (std::cin.eof())
            break;
        std::cout << "Send \'" << buffer << "\' to child." << std::endl;
        protocolWrite(proc, buffer);
        protocolRead(proc, buffer);
        std::cout << "Got from child: " << buffer << std::endl;
    }

    proc.close();
    int status;
    pid_t child_pid = wait(&status);
    if (WIFEXITED(status))
        std::cout << "Child process " << child_pid << " exited with code "
                  << WEXITSTATUS(status) << std::endl;
    else
        std::cout << "Process " << child_pid << "did't exit after wait()" << std::endl;
    return 0;
}
