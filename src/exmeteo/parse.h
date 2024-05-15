#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "req.h"

char*** currency__get_codes(char *api_key);
void free_2D_string_array(char ***stringList, int totalStrings);

#endif // !PARSE_H_INCLUDED
