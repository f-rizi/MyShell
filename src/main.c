#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <lexer.h>

#define PROMPT "myshell$ "

static int run_one_command(const char *cmdline)
{
    char **argv = lex(cmdline);
    if (!argv)
    {
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");

        for (char **p = argv; *p; p++)
        {
            free(*p);
        }

        free(argv);
        return 1;
    }

    // Is this child process?
    if (pid == 0)
    {
        execvp(argv[0], argv);
        perror("execvp");

        // Clean, proper exit for failed exec (127 = command not found)
        _exit(127);
    }

    int wstatus;
    waitpid(pid, &wstatus, 0);

    for (char **p = argv; *p; p++)
    {
        free(*p);
    }

    free(argv);

    // Did the child finished normally?
    if (WIFEXITED(wstatus))
    {
        return WEXITSTATUS(wstatus);
    }

    return 1;
}

int main(int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

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

        run_one_command(line);
    }

    free(line);
    return 0;
}