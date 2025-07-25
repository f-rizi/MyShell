#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PROMPT "myshell$ "

int main(int argc, char *argv[]) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    while (1)
    {
        printf(PROMPT);
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1) {
            putchar('\n');
            break;
        }

        printf("%s", line);
    }
    
    free(line);
    return 0;
}