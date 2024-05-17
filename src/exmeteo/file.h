#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
  #include <windows.h>
#elif __linux__
  #include <unistd.h>
  #include <dirent.h>
#else
  #error "Unsupported Operating System, use a normal os, loser" 
#endif

bool dir_exist(const char *dirname);
bool file_exists(const char *filename);
int create_dir(const char *dirname);
char *get_path(const char *filename);

#endif
