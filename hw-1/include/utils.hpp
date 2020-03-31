#ifndef PROC_UTILS_HPP
#define PROC_UTILS_HPP
#include <vector>
#include <string>

std::vector<std::string> parse_params(const std::string& path);
void get_pipe(int pipefd[2]);
int try_exec(const std::string &path);

#endif //PROC_UTILS_HPP
