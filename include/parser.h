#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

/**
 * A single command in a (possibly piped) pipeline.
 *
 * argv      – NULL-terminated array of argument strings
 * infile    – path for input redirection, or NULL
 * outfile   – path for output redirection, or NULL
 * append    – true if “>>” was used
 * background– true if command should run in background (‘&’)
 * next      – next command in pipeline, or NULL
 */
typedef struct cmd
{
    char **argv;
    char *infile;
    char *outfile;
    bool append;
    bool background;
    struct cmd *next;
} cmd_t;

/**
 * Parse the array of tokens into a linked list of cmd_t.
 * Returns NULL on parse error or OOM.
 * Caller must free with free_cmds().
 */
cmd_t *parse(char **tokens);

/**
 * Free a chain of cmd_t and all its allocated argv strings.
 */
void free_cmds(cmd_t *head);

#endif // PARSER_H
