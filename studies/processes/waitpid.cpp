#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t pid = fork();

	if (pid < 0)
		std::cout << "Troubles with fork()" << std::endl;
	else if (pid == 0)
	{
		std::cout << "Child PID: " << getpid() << std::endl;
		std::cout << "Parent PID: " << getppid() << std::endl;
		sleep(15);
		return 54;
	}
	else
	{
		int status;
		pid_t res = waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status))
			std::cout << "Process " << pid << " exited with code "
				<< WEXITSTATUS(status) << std::endl;
		else if (WIFSIGNALED(status))
			std::cout << "Process " << pid << " dumped with signal "
				<< WTERMSIG(status) << std::endl;
		else if (WIFSTOPPED(status))
			std::cout << "Process " << pid << " stopped with signal "
				<< WSTOPSIG(status) << std::endl;
	}
}
