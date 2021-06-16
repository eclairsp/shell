/*
	    Name: Prabhjyot Singh Dhillon
	    Student ID: 110026126
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 1000

void writeOut(char *buf)
{
    write(STDOUT_FILENO, buf, strlen(buf));
}

void childFunction(char *buf)
{
    writeOut("I am a child working for my parent\n");

    int a, b;
    char op;

    int err = sscanf(buf, "%d %c %d", &a, &op, &b);

    if (err != 3)
    {
        exit(50);
    }

    if ((op == '/') && (b == 0))
    {
        exit(100);
    }

    if (!(op == '+' || op == '-' || op == '*' || op == '/' || op == '%'))
    {
        exit(200);
    }

    int result;

    switch (op)
    {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        result = a / b;
        break;
    case '%':
        result = a % b;
        break;
    }

    char *rs;
    sprintf(rs, "%d %c %d = %d\n", a, op, b, result);
    writeOut(rs);

    exit(0);
}

int calculator()
{
    char *prompt = "This program makes simple arithmetics\n";
    writeOut(prompt);

    char *ari = "Enter an arithmetic statement, e.g., 34 + 132 > ";
    char buf[SIZE + 1];
    int status;
    while (1)
    {
        writeOut(ari);
        size_t sz = read(STDIN_FILENO, buf, SIZE);
        if (sz > 0)
        {
            buf[sz] = '\0';
        }

        if (!strcmp(buf, "quit\n"))
        {
            return 0;
        }

        int c = fork();

        if (c == 0)
        {
            childFunction(buf);
        }
        else
        {

            writeOut("Created a child to make your operation, waiting \n");
            wait(&status);

            if (WIFEXITED(status))
            {
                if (WEXITSTATUS(status) == 50)
                {
                    writeOut("Wrong statement\n");
                }

                if (WEXITSTATUS(status) == 100)
                {
                    writeOut("Division by zero\n");
                }

                if (WEXITSTATUS(status) == 200)
                {
                    writeOut("Wrong operator\n");
                }
            }
        }
    }
}
