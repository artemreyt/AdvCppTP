#include <iostream>
#include <signal.h>
#include <unistd.h>

int main();

void	sig_handler(int sig)
{
	std::cout << "Not now baby!" << std::endl;
	// signal(sig, SIG_IGN);
	// main();
}


int main()
{
	signal(SIGTERM, sig_handler);
	std::cout << "PID: " << getpid() << std::endl;
	while (1)
		sleep(60);
	return(0);
}
