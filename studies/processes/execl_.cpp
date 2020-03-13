#include <iostream>
#include <unistd.h>


int main()
{
	pid_t pid = fork();

	if (pid < 0)
		std::cout << "Troubles with fork()" << std::endl;
	else if (pid == 0)
	{
		/* CHILD */

		std::cout << "I'm Child. Executing \'cat -e hello.txt\'..." << std::endl;
		int res = execlp("cat", "cat", "-e", "hello.txt", NULL);
		std::cout << "Fail of executing. Res of exec is " << res << std::endl;
	}
	else
	{
		std::cout << "I'm Parent." << std::endl;
	}
	return 0;
}
