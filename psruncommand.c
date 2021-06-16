/*
        Name: Prabhjyot Singh Dhillon
        Student ID: 110026126
*/

#include "pssmallsh.h"
#include <unistd.h>
#include <stdlib.h>

/*
void doer(char **line) {
	if (!strcmp("calculator", line[0])) {
		//execvp("./calculator", NULL);
		calculator();
		exit(1);
	}

	execvp(line[0], line);
	perror(*line);
	exit(1);
}
*/

int runcommand(char **cline, int where)
{
	pid_t pid;
	int status;
	switch (pid = fork()) {
		case -1:
			perror("smallsh");
			return (-1);
		case 0:
			execvp(cline[0], cline);
			perror(*cline);
			exit(1);
	}
	//code for parents
	if (where == BACKGROUND)
	{
		printf("[process id %d]\n", pid);
		return (0);
	}
	
	if (waitpid(pid, &status, 0) == -1)
		return (-1);
	else
		return (status);
}
