#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

ssize_t _getline(char **lineptr, size_t *n, FILE *strem);
void execute_command(char *command);

#endif /* SHELL_H */
