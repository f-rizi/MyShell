#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>


void free_tokens_null_terminated(char **tokens);

void free_tokens_with_count(char **tokens, size_t count);

/**
 * Split 'line' into a NULL-terminated array of strings.
 * The caller owns both the array and each string.
 */
char **lex(const char *line);

#endif // LEXER_H