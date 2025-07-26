#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

/**
 * Split 'line' into a NULL-terminated array of strings.
 * The caller owns both the array and each string.
 */
char **lex(const char *line);

#endif // LEXER_H