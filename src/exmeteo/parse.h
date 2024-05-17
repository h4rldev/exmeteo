#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "color.h"

char*** currency__get_codes(const char *api_key, const char *filename_to_write);
void free_2D_string_array(char ***string_list, int total_strings);
char*** get_2D_array_from_json(json_t *json);

#endif // !PARSE_H_INCLUDED
