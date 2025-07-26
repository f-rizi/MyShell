#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"

cmd_t *parse(char **tokens)
{
    if (!tokens)
    {
        return NULL;
    }

    char **tkn = tokens;
    cmd_t *head = NULL, *current = NULL;

    while (*tkn)
    {
        cmd_t *cmd = calloc(1, sizeof(cmd_t));

        if (!cmd)
        {
            free_cmds(head);
            return NULL;
        }

        size_t cap = 4, cnt = 0;

        if (!head)
        {
            head = cmd;
        }
        else
        {
            current->next = cmd;
        }

        current = cmd;

        cmd->argv = malloc(cap * sizeof(char *));

        if (!cmd->argv)
        {
            free(cmd);
            free_cmds(head);
            return NULL;
        }

        while (*tkn && strcmp(*tkn, "|") != 0 && strcmp(*tkn, "&") != 0)
        {
            if (((strcmp(*tkn, ">") == 0) || (strcmp(*tkn, ">>") == 0)))
            {
                tkn++;

                if (!tkn)
                {
                    break;
                }

                if (strcmp(*tkn, ">>") == 0)
                {
                    cmd->append = true;
                }

                cmd->outfile = strdup(*tkn);
                tkn++;
            }
            else if ((strcmp(*tkn, "<") == 0))
            {
                tkn++;

                if (!tkn)
                {
                    break;
                }

                cmd->infile = strdup(*tkn);
                tkn++;
            }
            else
            {
                if (cnt + 1 >= cap)
                {
                    cap *= 2;
                    char **tmp = realloc(cmd->argv, cap * sizeof(char *));

                    if (!tmp)
                    {
                        free_cmds(head);
                        return NULL;
                    }

                    cmd->argv = tmp;
                }

                cmd->argv[cnt] = strdup(*tkn);

                if (!cmd->argv[cnt])
                {
                    free_cmds(head);
                    return NULL;
                }

                tkn++;
                cnt++;
            }
        }

        cmd->argv[cnt] = NULL;

        if (*tkn && strcmp(*tkn, "&") == 0)
        {
            tkn++;
            cmd->background = true;
        }

        if (*tkn && strcmp(*tkn, "|") == 0)
        {
            tkn++;
        }
    }

    return head;
}

void free_cmds(cmd_t *head)
{
    cmd_t *cmd = head;

    while (cmd)
    {
        cmd_t *n = cmd->next;

        if (cmd->argv)
        {
            for (char **p = cmd->argv; *p; p++)
            {
                free(*p);
            }

            free(cmd->argv);
        }

        free(cmd->infile);
        free(cmd->outfile);
        free(cmd);

        cmd = n;
    }
}