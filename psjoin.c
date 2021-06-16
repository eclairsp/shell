/*
        Name: Prabhjyot Singh Dhillon
        Student ID: 110026126
*/

#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int join(char *com1[], char *com2[]) {
	//printf("beofre  0: %s 1: %s\n", com1[0], com1[1]);
	//printf("after  0: %s 1: %s\n", com2[0], com2[1]);
	int p[2], status;

	switch (fork()) {
		case -1: 
			perror("1st fork call in join");
			exit(3);
		case 0: 
			break;
		default: 
			wait(&status);
			return(status);
	}
	
	//child process's code
	if (pipe(p) == -1){
		perror("pipecall in join ");
		exit(4);
	}

	switch (fork()) {
		case -1:
			perror("2nd fork call failed in join");
			exit(5);
		case 0:  //the writing process (grandchild procss)
			dup2(p[1], 1);  //redirect stdout to the pipe, writing end.
				
			close(p[0]);
			close(p[1]);

			execvp(com1[0], com1);

			perror("1st execvp call failed in join");
			exit(6);
		default: //the "parent" process's code
			dup2(p[0], 0); //redirect stdin to the pipe, reading end

			close(p[0]);
			close(p[1]);

			execvp(com2[0], com2);

			perror("2nd execvp call failed in join");
			exit(7);
		}
}
