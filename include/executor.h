#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

/**
 * Execute a pipeline of cmd_t’s:
 *  – For built-ins (cd, exit, etc.), run them in-process.
 *  – For external commands, fork + execvp().
 *  – Wire up pipes between each cmd->next stage.
 *  – Apply redirections (infile, outfile, append).
 *  – If background==false, wait for the last child and return its exit code.
 *  – If background==true on the final stage, do not wait; return 0 immediately.
 *
 * Returns the exit status of the foreground pipeline, or 0 if backgrounded.
 */
int exec_cmds(cmd_t *head);

#endif // EXECUTOR_H
