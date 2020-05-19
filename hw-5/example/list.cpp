//
// Created by Artem Starshov on 2020-04-23.
//

#include "Allocator.hpp"
#include <list>
#include <iostream>
#include <memory>

int main() {
    char *mem = new char [100];
    shmem::AllocState *state = reinterpret_cast<shmem::AllocState*>(mem);
    state->start = (char*)state + sizeof(shmem::AllocState);
    state->end = (char*)state + 100;

    shmem::Allocator<int> alloc(state);

    std::list<int, shmem::Allocator<int>> lst(alloc);
    lst.push_back(25);
    lst.push_back(89);

    for (auto &elem: lst) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    return 0;
}
