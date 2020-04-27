#include "Map.hpp"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include "thread"


int main() {
    shmem::Map<shmem::string<char>, int> map;

    const std::string key2 = "parent Two and loooooong string here";
    const std::string key3 = "child Three";
    const std::string key7 = "parent Seven and loooooong string here";
    const std::string key9 = "child Nine and loooooong string here";

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "fork error" << std::endl;
        return 1;
    } else if (pid == 0) {

        map.insert(key9, 9);
        map.insert(key3, 3);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << getpid() << " (child): " << map.at(key2) << std::endl;
        std::cout << getpid() << " (child): " << map.at(key7) << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        map.insert(key7, 9292);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        map.erase(key7);
    } else {
        map.insert(key2, 2);
        map.insert(key7, 7);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << getpid() << " (parent): " << map.at(key3) << std::endl;
        std::cout << getpid() << " (parent): " << map.at(key9) << std::endl;

        while (true) {
            try {
                std::cout << getpid() << " (parent) key7: " << map.at(key7) << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(3));
            } catch (const std::out_of_range& ex) {
                std::cout << "Key7 was erased by child process" << std::endl;
                break;
            }
        }
    }
}