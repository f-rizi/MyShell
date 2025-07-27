#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lexer.h"

void free_tokens_null_terminated(char **tokens)
{
    char **ptr = tokens;

    while (*ptr)
    {
        char **next = (ptr + 1);
        free(*tokens);
        ptr = next;
    }

    free(tokens);
}

void free_tokens_with_count(char **tokens, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        free(tokens[i]);
    }

    free(tokens);
}

static int has_matching_quote(const char *line, size_t start, size_t len, char quote)
{
    for (size_t i = start; i < len; i++)
    {
        if (line[i] == quote)
        {
            return 1;
        }
    }

    return 0;
}

char **lex(const char *line)
{
    size_t len = strlen(line);
    size_t idx = 0;
    size_t toks_cap = 8, toks_cnt = 0;

    char **tokens = malloc(toks_cap * sizeof *tokens);
    if (!tokens)
    {
        return NULL;
    }

    while (idx < len)
    {
        while (idx < len && isspace((unsigned char)line[idx]))
        {
            idx++;
        }

        if (idx >= len)
        {
            break;
        }

        size_t buf_cap = 8, buf_len = 0;

        char *buf = malloc(buf_cap);
        if (!buf)
        {
            free_tokens_with_count(tokens, toks_cnt);
            return NULL;
        }

        while (idx < len && !isspace((unsigned char)line[idx]))
        {
            char ch = line[idx];

            if (ch == '\\')
            {
                if (idx + 1 < len)
                {
                    idx++;
                    ch = line[idx];
                }
                idx++;
            }
            else if ((ch == '"' || ch == '\'') && (buf_len == 0 || has_matching_quote(line, idx + 1, len, ch)))
            {
                char quote = ch;
                idx++;

                while (idx < len && line[idx] != quote)
                {
                    if (line[idx] == '\\' && idx + 1 < len)
                    {
                        idx++;
                        ch = line[idx];
                    }
                    else
                    {
                        ch = line[idx];
                    }

                    idx++;

                    if (buf_len + 1 >= buf_cap)
                    {
                        buf_cap *= 2;

                        char *tmp = realloc(buf, buf_cap);

                        if (!tmp)
                        {
                            free(buf);
                            free_tokens_with_count(tokens, toks_cnt);
                            return NULL;
                        }

                        buf = tmp;
                    }

                    buf[buf_len++] = ch;
                }

                if (idx < len && line[idx] == quote)
                {
                    idx++;
                }

                continue;
            }
            else
            {
                idx++;
            }

            if (buf_len + 1 >= buf_cap)
            {
                buf_cap *= 2;
                char *tmp = realloc(buf, buf_cap);

                if (!tmp)
                {
                    free(buf);
                    free_tokens_with_count(tokens, toks_cnt);
                    return NULL;
                }

                buf = tmp;
            }

            buf[buf_len++] = ch;
        }

        if (buf_len > 0)
        {
            buf[buf_len] = '\0';

            if (toks_cnt + 1 >= toks_cap)
            {
                toks_cap *= 2;
                char **tmp = realloc(tokens, toks_cap * sizeof *tokens);

                if (!tmp)
                {
                    free(buf);
                    free_tokens_with_count(tokens, toks_cnt);
                    return NULL;
                }

                tokens = tmp;
            }

            tokens[toks_cnt++] = buf;
        }
        else
        {
            free(buf);
        }
    }

    char **tmp = realloc(tokens, (toks_cnt + 1) * sizeof *tokens);
    if (tmp)
    {
        tokens = tmp;
    }

    tokens[toks_cnt] = NULL;

    return tokens;
}
