#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "req.h"

char *currency__get_json_value(char *api_key, char *value);

#endif // !PARSE_H_INCLUDED
