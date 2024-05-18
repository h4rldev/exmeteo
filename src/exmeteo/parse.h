#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <math.h>
#include <stdbool.h>

void free_2D_string_array(char ***string_list, int total_strings);
char*** get_2D_array_from_json(json_t *json);

char*** currency__get_codes(const char *api_key, const char *filename);
float currency__get_conversion_rate(const char *cur1, const char *cur2, const char* api_key);

char *weather__get_weather_data_w_format(char *location, char *format);
char *weather__get_weather_data(char* location, bool detailed, bool trim);

#endif // !PARSE_H_INCLUDED
