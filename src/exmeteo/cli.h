#ifndef CLI_H_INCLUDED
#define CLI_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void compare_flags(int argc, char **argv, char *result[]);
int init(int argc, char **argv);

#endif // !CLI_H_INCLUDED
