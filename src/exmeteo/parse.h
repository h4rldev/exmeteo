#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "req.h"

cJSON *currency__get_json_value(char *api_key, char *value);
char*** currency__get_codes(char *api_key);
void free_2D_string_array(char ***stringList, int totalStrings);

#endif // !PARSE_H_INCLUDED
