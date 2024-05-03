#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32

#define CACHE_DIR() getenv("APPDATA") "\\exmeteo\\"

#elif __linux__

#define CACHE_DIR() getenv("HOME") "/.cache/exmeteo/"

#else
  #error "Unsupported Operating System, use a normal os, loser"
#endif

cJSON *read_from_cache(FILE *cache);

int write_to_json(char *filename, cJSON *json);
int update_cache(char *api_key);
int clear_cache();

#endif
