#include "Map.hpp"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include "thread"


int main()
{
    shmem::Map<shmem::string<char>, int> map;


    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "fork error" << std::endl;
        return 1;
    } else if (pid == 0) {
        map["child Nine"] = 9;
        map["child Three"] = 3;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << getpid() << " (child): " << map.at("parent Two") << std::endl;
        std::cout << getpid() << " (child): " << map.at("parent Seven") << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        map.erase("parent Seven");
    } else {
        map["parent Two"] = 2;
        map["parent Seven"] = 7;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << getpid() << " (parent): " << map.at("child Three") << std::endl;
        std::cout << getpid() << " (patent): " << map.at("child Nine") << std::endl;

        while (true) {
            try {
                std::cout << getpid() << " (parent): " << map.at("parent Seven") << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(3));
            } catch (const std::out_of_range &ex) {
                std::cout << "Key " << "parent Seven" << " was erased by child process" << std::endl;
                break;
            }
        }
    }
}