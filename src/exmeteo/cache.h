#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#define HOME getenv("APPDATA")
#define CACHE_DIR "\\exmeteo\\"

#elif __linux__

#define HOME getenv("HOME")
#define CACHE_DIR "/.cache/exmeteo/"

#else
  #error "Unsupported Operating System, use a normal os, loser"
#endif

json_t *read_from_cache(char *cache_name);

int write_to_json(char *filename, json_t *json);
int update_cache(char *api_key);
int clear_cache();

#endif
