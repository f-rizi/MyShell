#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/executor.h"

static int run_one_command(cmd_t *cmd)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    // Is this child process?
    if (pid == 0)
    {
        if (cmd->infile)
        {
            int fd_in = open(cmd->infile, O_RDONLY);

            if (fd_in < 0)
            {
                perror(cmd->infile);
                _exit(1);
            }

            dup2(fd_in, STDIN_FILENO);

            if (dup2(fd_in, STDIN_FILENO) < 0)
            {
                perror("dup2");
                _exit(1);
            }

            close(fd_in);
        }

        if (cmd->outfile)
        {
            int flags = O_CREAT | O_WRONLY | (cmd->append ? O_APPEND : O_TRUNC);
            int fd_out = open(cmd->outfile, flags, 0666);

            if (fd_out < 0)
            {
                perror(cmd->infile);
                _exit(1);
            }

            dup2(fd_out, STDOUT_FILENO);

            if (dup2(fd_out, STDIN_FILENO) < 0)
            {
                perror("dup2");
                _exit(1);
            }
            close(fd_out);
        }

        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");

        // Clean, proper exit for failed exec (127 = command not found)
        _exit(127);
    }

    int wstatus;
    waitpid(pid, &wstatus, 0);

    // Child finished normally?
    if (WIFEXITED(wstatus))
    {
        return WEXITSTATUS(wstatus);
    }

    return 1;
}

int exec_cmds(cmd_t *head)
{
    if (!head)
    {
        return 0;
    }

    cmd_t *cmd = head;

    if (!cmd->next)
    {
        return run_one_command(cmd);
    }

    return 0;
}