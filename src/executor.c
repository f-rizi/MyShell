#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/executor.h"

int exec_cmds(cmd_t *head)
{
    if (!head)
    {
        return 0;
    }

    cmd_t *cmd = head;

    while (cmd)
    {

        cmd = cmd->next;
    }

    return 0;
}