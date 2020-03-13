#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
	* Child Process writes to pipe
	* Parent listens
*/

int main()
{
	int pipefd[2];
	pid_t cpid;

	if (pipe(pipefd) < 0)
	{
		perror("Pipe error");
		exit(1);
	}

	printf("pipefd: %d %d\n", pipefd[0], pipefd[1]);
	cpid = fork();
	if (cpid == -1)
	{
		perror("fork problems");
		exit(1);
	}
	else if (cpid > 0)
	{
		close(pipefd[0]);
		
		char msg[100];
		sprintf(msg, "Hello, I'm parent process with PID: %d!", getpid());

		/*
			* just write to pipefd[1]
			* ---------------------------------------------
		*/
		// write(pipefd[1], msg, strlen(msg));
		// ------------------------------------------------


		/*
			* change stdout to pipefd[1]
			* ---------------------------------------------
		*/
		if (dup2(pipefd[1], STDOUT_FILENO) != STDOUT_FILENO)
		{
			perror("dup2");
			exit(1);
		}
		while (1)
		{
			gets(msg);
			write(STDOUT_FILENO, msg, strlen(msg));
			write(STDOUT_FILENO, "\n", 1);
			// puts(msg);
		}
		// ------------------------------------------------

		exit(0);
	}
	else
	{
		close(pipefd[1]);

		char buf[100];

		/*
			* just read from pipefd[0]
			* ---------------------------------------------
		*/
		// ssize_t was_read = read(pipefd[0], buf, 100);
		// buf[was_read] = '\0';
		// ------------------------------------------------


		/* 
			* change stdin to pipefd[0]
			* ---------------------------------------------
		*/
		if (dup2(pipefd[0], STDIN_FILENO) != STDIN_FILENO)
		{
			perror("dup2");
			exit(1);
		}

		execlp("cat", "cat", "-e", NULL);
		perror("execlp");
		// gets(buf);
		// ssize_t was_read = strlen(buf);
		// // -------------------------------------------------


		// printf("Was read %ld bytes from pipe\n", was_read);
		// printf("Message includes: %s", buf);
		exit(0);
	}
}
