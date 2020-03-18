#include <iostream>
#include "Process.hpp"
#include <string>
#include <string.h>
#include "Protocol.hpp"
#include <sys/wait.h>
#include <unistd.h>


/*
    * this process scans lines from stdin;
    * sends it to child process via protocol "MSG_SIZE|MSG";
    * child process returns sended message in UPPERCASE;
    * this process prints on STDOUT recieved message;
    * this process ends when stdin eof found;
*/

int main()
{
    artemreyt::Process proc("./child.out");
    std::string buffer;

    while (true)
    {
        std::getline(std::cin, buffer);
        if (std::cin.eof())
            break;
        std::cout << "Send \'" << buffer << "\' to child." << std::endl;

        if (Protocol::write(proc.getStdin(), buffer) == -1)
        {
            std::cout << "Error while writing to child" << std::endl;
            break;
        }

        if (Protocol::read(proc.getStdout(), buffer) == -1)
        {
            std::cout << "Error while reading from child" << std::endl;
            break;
        }

        std::cout << "Got from child: " << buffer << std::endl;
    }
    proc.close();
    return 0;
}
