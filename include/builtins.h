#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdbool.h>

/**
 * Return true if the command name is one of the shell's built-ins.
 */
bool is_builtin(const char *cmd);

/**
 * Execute the built-in command in-process.
 * argc/argv are conventional; return its exit code.
 */
int run_builtin(int argc, char *argv[]);

#endif // BUILTINS