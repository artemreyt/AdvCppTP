#ifndef PROC_UTILS_HPP
#define PROC_UTILS_HPP
#include <vector>
#include <string>

std::vector<char *> parse_params(const std::string& path);
void get_pipe(int pipefd[2]);

#endif //PROC_UTILS_HPP
