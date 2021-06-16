/*
        Name: Prabhjyot Singh Dhillon
        Student ID: 110026126
*/

#include "psjoin.c"
#include "psredirect.c"
#include "pscalculator.c"

#include "pssmallsh.h"
#include <string.h>
#include <stdlib.h>

int gettok(char **outptr, int *count, int *pos, int *present, int *tok_type);
int runcommand(char **cline, int where);


int commandcheck(char **line) {
	/* 
		3. Exit or quit
		if user types exit or quit end the shell.
	*/
	if(strcmp("quit", line[0]) == 0 || strcmp("exit", line[0]) == 0) {
		exit(1);
	}
	
	/* 
		1. Change working directory
		using the output from gettok to check if command is 'cd'
		if it is then try to change directory
	*/
	if (!strcmp("cd", line[0])) {
		//printf("0:%s 1: %s", line[0], line[1]);
		int s;
		if (line[1] == NULL) {
			chdir(getenv("HOME"));
		} else {
			s = chdir(line[1]);
		}
		if (s < 0) {
			printf("Error occured while changing directory\n");
			//printf("Exiting!");
			return 0;
		}
		return 0;
	}
	
	/* 
		7. Calculator
		there are two ways to do this either compile the calculator program and execute it
		or here we are directly calling the function
	*/
	if (!strcmp("calculator", line[0])) {
                //execvp("./calculator", NULL);
                calculator();
                return 0;
        }
}

/* 
	5 and 6. Pipe and redirect text
	based on the values we got from gettok create array of commands
	before and after the pipe or redirect command
*/
int commandExtra(int count, int pos, int tok_type, char **arg) {
	char *before[MAXARG + 1];
	char *after[MAXARG + 1];

	int  i = 0;
	for(i; i<count; i++) {
		//printf("%d: %s %d\n", i, arg[i], posit);
		if( i < pos -1) {
			*(before + i) = arg[i];
			//printf("bef: %d : %s\n", i, before[i]);
		}
		if( i == pos - 1) {
			*(before + i) = NULL;
			//printf("bef: %d : %s\n", i, before[i]);
		}
		if( i > pos  - 1) {
			*(after + i - pos) = arg[i];
			//printf("aft: %d : %s\n", i - pos, after[i]);
		}
		if( i == count - 1) {
			*(after + i - pos) = NULL; 
			//printf("aft: %d : %s\n", i - pos, after[i]);
		}
	}
	
	/* 
		5 and 6. Pipe and redirect text
		if command is PIPE call join
		else call TEXT
		can be easily expanded for more commands if needed in the future
	*/
	switch(tok_type) {
		case PIPE: 
			join(before, after);
			break;
		case TEXT: 
			redirect(before, after);
			break;
		default:
			break;
	}
}

void procline(void) 
{
	char *arg[MAXARG + 1]; 
	int toktype; 
	int narg; 
	int type; 

	/* 
		5 and 6. Pipe and redirect text
		declaring variables used for these commands
		count = total number of tokens in the command based on space from gettok
		present = to check if either pipe or redirect command is given by user
		pos = in all the tokens at which position is the pipe or redirect command
		tok_type = whether it is a pipe or token command
		
	*/
	int count = 0;
	int present = 0;
	int pos;
	int tok_type;
	narg = 0;

	for (;;) 
	{
		/* 
			5 and 6. Pipe and redirect text
			sending the addresses to the gettok function of the variables
			gettok can then assign values to these variables
			check comments in gettok for more info 
		*/
		toktype = gettok(&arg[narg], &count, &pos, &present, &tok_type);
		switch (toktype) {
			case ARG:
				if (narg < MAXARG) {
					narg++;
				}
				break;
			case EOL:
			case SEMICOLON:
			case AMPERSAND:
				if (toktype == AMPERSAND)
					type = BACKGROUND;
				else
					type = FOREGROUND;
				if (narg != 0) {
					arg[narg] = NULL;
					/* 
						if the command is either cd or quit check if they are done and return
					*/
					if (commandcheck(arg) == 0) {
						return;
					}	
					/* 
						5 and 6. Pipe or redirect text
						if these commands are present run the function 
					*/ 				
					if(present) {
						commandExtra(count, pos, tok_type, arg);
						return;
					}
					runcommand(arg, type);
				}
				if (toktype == EOL)
					return;
				narg = 0;
				break;
	  	}
	}
	
	//printf("%d", narg);
}
