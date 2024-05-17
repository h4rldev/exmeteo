#include "cache.h"
#include "color.h"
#include "file.h"
#include <stdio.h>
 
json_t *read_from_cache(char *cache_name) {
  char *path = get_path(cache_name);
  
  FILE *cache_file = fopen(path, "r");
  if (!cache_file) {
    fprintf(stderr, RED "!%s Can't open file.\n", CLEAR);
    return 0;
  }
  
  fseek(cache_file, 0, SEEK_END); // Move to the end of the file
  long fsize = ftell(cache_file); // Get the current position (size)
  rewind(cache_file); // Reset the file pointer to the beginning

  char* buffer = (char*)malloc(fsize + 1);
  if (!buffer) {
    fprintf(stderr, RED "!%s Failed to allocate content buffer.\n", CLEAR);
    fclose(cache_file);
    return 0;
  }
  
  fread(buffer, 1, fsize, cache_file);
  buffer[fsize] = '\0';

  size_t amount_read = fclose(cache_file);
  if (amount_read <= 1) {
    printf(stderr, RED "!%s File didn't read properly.\n", CLEAR);
    free(buffer);
    free(path);
    return 0;
  }

  printf("buffer: %s", buffer);

  free(buffer);
  free(path);
  return 0;
} 

int write_to_json(char *filename, json_t *json) {
  char* path = get_path(filename);

  char *json_str = json_dumps(json, 0);
  if (! json_str) {
    fprintf(stderr, RED "!%s Error while dumping JSON.\n", CLEAR);
    return 1;
  }

  FILE *file = fopen(path, "w");
  if (!file) {
      fprintf(stderr, RED "!%s Failed to open file in write mode.\n", CLEAR);
      free(json_str);
      return 1;
  }

  fputs(json_str, file);
  fclose(file);

  free(json_str);
  free(path);

  json_decref(json);

  return 0;
}
