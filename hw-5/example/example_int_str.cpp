#include "Map.hpp"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include "thread"


int main() {
    shmem::Map<int, shmem::string<char>> map;


    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "fork error" << std::endl;
        return 1;
    } else if (pid == 0) {

        map.insert(9, "child Nine and loooooong string here");
        map.insert(3, "child Three");

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << getpid() << " (child): " << map.at(2) << std::endl;
        std::cout << getpid() << " (child): " << map.at(7) << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        map.insert(7, "i'm child and i'm changing key7 value!");

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        map.erase(7);
    } else {
        map.insert(2, "child Two and loooooong string here");
        map.insert(7, "child Seven and very very loooooong string here");

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << getpid() << " (parent): " << map.at(3) << std::endl;
        std::cout << getpid() << " (parent): " << map.at(9) << std::endl;

        while (true) {
            try {
                std::cout << getpid() << " (parent) key 7: " << map.at(7) << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(3));
            } catch (const std::out_of_range& ex) {
                std::cout << "Key " << 7 << " was erased by child process" << std::endl;
                break;
            }
        }
    }
}