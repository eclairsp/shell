/*
        Name: Prabhjyot Singh Dhillon
        Student ID: 110026126
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void redirect(char *before[], char *after[]) {
	int file = open(after[0], O_WRONLY | O_CREAT, 0700);
	
	if (file == -1) {
		printf("Cannot open file\n");
		return;
	}

	if (!fork()) {
		close(1);
		dup(file);
		execvp(before[0], before);
	} else {
		close(file);
		printf("\nSaved to file %s\n", after[0]);
		wait(NULL);
	}	
	return;
}
