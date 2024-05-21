#include "cache.h"
#include "color.h"
#include "file.h"

/*
 * json_t read_from_cache(const char *cache_name)
 * 
 * Reads, and parses the json defined in %APPDATA\\exmeteo\\ or in ~/.cache/exmeteo/ 
 * 
 * Usage:
 * json_t *cache_codes = read_from_cache("cache.json");
 * char *content = json_dump(cache_codes, 0);
 * printf("JSON content: %s", content);
 */

json_t *read_from_cache(const char *cache_name) {
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
  
  size_t amount_read = fread(buffer, 1, fsize, cache_file);
  if (amount_read < 1) {
    fprintf(stderr, RED "!%s Failed to read file properly.\n", CLEAR);
    fclose(cache_file);
    free(buffer);
    free(path);
    return 0;
  }

  buffer[fsize] = '\0';
  fclose(cache_file); 

  json_t *codes;
  json_error_t error;
  codes = json_loads(buffer, 0, &error);
  if (!codes) {
    fprintf(stderr, RED "!%s Error parsing JSON on line: %d: %s\n", CLEAR, error.line, error.text);
    free(buffer);
    return 0;
  }

  if (!json_is_array(codes)) {
    fprintf(stderr, RED "!%s JSON not array?\n", CLEAR);
    free(buffer);
    return 0;
  }
   
  free(buffer);
  free(path);
  return codes;
} 

/*
 * int write_to_json(const char *filename, json_t *json, bool free_json)
 * 
 * Reads, and writes json to %APPDATA\\exmeteo\\ or in ~/.cache/exmeteo/ 
 * 
 * Usage:
 * if (write_to_json("cache.json", root, false) == 1) {
 *    - Do some error handling
 * } 
 */

int write_to_json(const char *filename, json_t *json, bool free_json) {
  char* path = get_path(filename);

  char *json_str = json_dumps(json, 0);
  if (! json_str) {
    fprintf(stderr, RED "!%s Error while dumping JSON.\n", CLEAR);
    free(path);
    return 1;
  }

  FILE *file = fopen(path, "w");
  if (!file) {
    fprintf(stderr, RED "!%s Failed to open file in write mode.\n", CLEAR);
    free(json_str);
    free(path);
    return 1;
  }

  fputs(json_str, file);
  fclose(file);

  free(json_str);
  free(path);

  if (free_json) {
    json_decref(json);
  }

  return 0;
}
