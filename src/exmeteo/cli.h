#ifndef CLI_H_INCLUDED
#define CLI_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "metadata.h"

#ifdef COLORS
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define CLEAR   "\033[0m"
#endif

char *getUsername(void);
int compare_flags(char *argv[]);
int init(int argc, char *argv[]);

#endif // !CLI_H_INCLUDED
