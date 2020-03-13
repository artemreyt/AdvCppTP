#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int pipefd[2];
	pipe(pipefd);
	if (fcntl(pipefd[1], F_GETFL) != -1)
		puts("Pipe is readble");
	else
		puts("Pipe isn't readble");
	close(pipefd[1]);
	if (fcntl(pipefd[1], F_GETFL) != -1)
		puts("Pipe is readble");
	else
		puts("Pipe isn't readble");
}
