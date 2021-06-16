/*
        Name: Prabhjyot Singh Dhillon
        Student ID: 110026126
*/

#include "pssmallsh.h"
#include <signal.h>

// command arg1 arg2 arg3
static int timed_out;
static char inpbuf[MAXBUF], tokbuf[2 * MAXBUF],
*ptr = inpbuf, *tok = tokbuf;

int inarg(char c);

void catch(int sig) {
	timed_out = TRUE;
	memset(inpbuf, 0 , sizeof(inpbuf));
	printf(" Hurry! Press enter to continue\n");
}

int userin()
{
	/* 
		4. Alarm for annoying user
		setting the function for when signal happens 
		and setting timed_out to 'false'
	*/
	signal(SIGALRM, catch);
	timed_out = FALSE;
	
	int c, count;
	ptr = inpbuf;
	tok = tokbuf;
	char *p;
	char prompt[PATH_MAX];	
	/* 
		2. current working directory
		get the current working directory and print it.
	*/
	if(getcwd(prompt, sizeof(prompt)) != NULL) {
		printf("\nYou have 10 seconds to do something otherwise restarting...\n\n");
		printf("%s$ ", prompt);
		p = prompt; 
		/* 
			4. Alarm for annoying user
			setting the alarm as soon as working directory is set
		*/
		alarm(TIMEOUT);
	} else {
		printf("Couldn't get the directory\n");
		printf("Exiting\n");
		return(EOF);
	}
	//printf("%s$ ", p);
	count = 0;
	while (1)
	{
		if ((c = getchar()) == EOF)
			return(EOF);
			/* 
				4. Alarm for annoying user
				end the alarm if EOF reached
			*/
			alarm(0);
		if (count < MAXBUF)
			inpbuf[count++] = c;
		if (c == '\n' && count < MAXBUF)
		{
			inpbuf[count] = '\0';
			return count;
		}
		if (c == '\n') 
		{
			printf("smallsh: input line too long\n");
			count = 0;
			printf("\nYou have 10 seconds to do something otherwise restarting...\n\n");
			printf("%s$ ", p);
		}
		/* 
			4. Alarm for annoying user
			set the count back to start and clear the array.
		*/
		if (timed_out) {
			count = 0;
			memset(inpbuf, 0 , sizeof(inpbuf));
		}
	}

	//printf("inpbuf=%s\n", inpbuf);
}

/* 
	5 and 6. Pipe and redirect text
	get the addresses from proc_line.c and updata them
*/
int gettok(char **outptr, int *count, int *pos, int *present, int *tok_type)
{
	int type, ct;
	*outptr = tok;
	ct = *count;	
	while (*ptr == ' ' || *ptr == '\t')
		ptr++;
		ct++;
		*count= ct;

	*tok++ = *ptr;
	switch (*ptr++) {
	case '\n':
		type = EOL;
		break;
	case '&':
		type = AMPERSAND;
		break;
	case ';':
		type = SEMICOLON;
		break;
	/* 
		5 and 6. Pipe and redirect text
		check if pipe or redirect command is given by user
		and set the flag to show command is given 
		next  position where command is given based on spaces
		and finally whcih command is given
		'TEXT' and 'PIPE' ared defined in the pssmallsh.h file	
	*/
	case '>': 
		*pos = ct;
		*present = 1;
		*tok_type = TEXT;
		break;
	case '|':
		*pos = ct;
		*present = 1;
		*tok_type = PIPE;
		break;
	default:
		type = ARG;		
		while (inarg(*ptr))
			*tok++ = *ptr++;
	}
	*tok++ = '\0';
	return type;
}

//****************************************************************************************
static char special[] = { ' ', '\t', '&', ';', '\n', '\0' };
int inarg(char c)
{
	char *wrk;
	for (wrk = special; *wrk; wrk++)
	{
		if (c == *wrk)
			return (0);
	}
	return (1);
}


