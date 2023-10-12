#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_prompt(void);
ssize_t get_input(char **buffer, size_t *bufsize);
void handle_input_error(char *buffer);
pid_t fork_and_execute(char *buffer);
void wait_for_child(void);

#endif /* SHELL_H */
