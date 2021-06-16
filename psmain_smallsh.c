/*
        Name: Prabhjyot Singh Dhillon
        Student ID: 110026126
*/

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <string.h>

#include "pssmallsh.h"
#include "psproc_line.c"
#include "psuserin.c"
#include "psruncommand.c"

int userin();
void procline(void);

int main()
{
	while (1) {
		if(userin() != EOF) {
			procline();
		}
	}
}

