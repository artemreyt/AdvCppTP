#include <iostream>
#include <unistd.h>

int main()
{
	pid_t pid = fork();

	switch (pid)
	{
		case -1:
			std::cerr << "Failure while making fork()" << std::endl;
			break;
		case 0:
			std::cout << "I'm child! My PID is " << getpid() <<
				" and my PPID is " << getppid() << std::endl;
			while (getppid() != 1)
			{
				std::cout << "Parent Process is alive" << std::endl;
				sleep(1);
			}

			for (size_t i = 0; i < 4; i++)
			{
				std::cout << "Parent Process is dead :(" << std::endl;
				sleep(1);
			}
			break;
	
		default:
			std::cout << "I'm a parent! My PID is " << getpid() <<
				" and my PPID is " << getppid() << std::endl;
			sleep(5);
			std::cout << "I'M PARENT AND I'M DYING" << std::endl;
	}
	return (0);
}
