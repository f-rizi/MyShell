#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <../include/parser.h>
#include <../include/lexer.h>
#include <../include/executor.h>

#define PROMPT "rzsh$ "

void handle_sigint(int sig)
{
    printf("\n%s", PROMPT);
    fflush(stdout);
}

void handle_sigchld(int sig)
{
    printf("\nCaught signal %d (Ctrl+C)\n", sig);

    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
    }
}

int main(int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    signal(SIGCHLD, handle_sigchld);
    signal(SIGINT, handle_sigint);

    while (1)
    {
        printf(PROMPT);
        fflush(stdout);

        nread = getline(&line, &len, stdin);

        if (nread == -1)
        {
            putchar('\n');
            break;
        }

        char **tokens = lex(line);
        cmd_t *cmds = parse(tokens);

        exec_cmds(cmds);
        free_cmds(cmds);
    }

    return 0;
}