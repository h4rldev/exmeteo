#ifndef CLI_H_INCLUDED
#define CLI_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "metadata.h"
#include "color.h"

char *getUsername(void);
int init(int argc, char **argv);

#endif // !CLI_H_INCLUDED
