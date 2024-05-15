#include "parse.h"
#include <jansson.h>

json_t *currency__get_json_value(char* api_key) {
  char url[80];
  char *codes_response;
  snprintf(url, 80, "https://v6.exchangerate-api.com/v6/%s/codes", api_key);

  printf("url = %s\n", url);
  
  char *response = req(url);
  size_t response_length = (size_t)strlen(response) + 1;
  
  json_t *json = json_loads(response, response_length, 0); // this leaks 381 bytes.
  if (! json) {
    fprintf(stderr, RED "!%s Error while parsing JSON.\n", CLEAR);
    json_decref(json);
    return 0;
  }
  free(response);
  return json;
}

void free_2D_string_array(char ***stringList, int totalStrings) {
  for (int i = 0; i < totalStrings; i++) {
    for (int j = 0; j < 2; j++) {
      //printf("Freeing array[%d][%d]\n", i, j);
      free(stringList[i][j]);
    }
    //printf("Freeing array[%d]", i);
    free(stringList[i]);
  }
  //printf("freeing list.");
  free(stringList);
}

char*** currency__get_codes(char *api_key) {
  json_t *root = currency__get_json_value(api_key);
  json_t *codes = json_object_get(root, "supported_codes");
  if (!codes || !json_is_array(codes)) {
    fprintf(stderr, RED "!%s Codes is supposed to exist / suppose to be an array, returning 0..\n", CLEAR);
    json_decref(codes);
    return 0;
  }
  // Determine the size of the JSON array
  int size = json_array_size(codes);
    // Allocate memory for the 2D array
  char ***currency_codes = (char ***)malloc((size * sizeof(char **)));
  if (!currency_codes) {
    fprintf(stderr, RED "!%s Can't allocate memory for currency_codes, returning 0..\n", CLEAR);
    free(currency_codes);
    return 0;
  }
    // Iterate over the JSON array and store the strings in the 2D array
  for (int i = 0; i < size; i++) {
    json_t *item = json_array_get(codes, i);
    json_t *code = json_array_get(item, 0);
    json_t *name = json_array_get(item, 1);
    
    const char *code_str = json_string_value(code);
    const char *name_str = json_string_value(name);

    int code_size = (strlen(code_str) + 1);
    int name_size = (strlen(name_str) + 1);
    
    currency_codes[i] = (char **)malloc((2 * sizeof(char *)));
    //printf("Allocating currency_codes[%d][0]..", i);
    currency_codes[i][0] = (char *)malloc(code_size); // +1 for the null terminator
    //printf("Allocating currency_codes[%d][1]..", i);
    currency_codes[i][1] = (char *)malloc(name_size); // +1 for the null terminator


    strncpy(currency_codes[i][0], code_str, code_size);
    strncpy(currency_codes[i][1], name_str, name_size);
  }

  // Print the 2D array to verify
  // for (int i = 0; i < size; i++) {
  //  printf("[\"%s\",\"%s\"]\n", currency_codes[i][0], currency_codes[i][1]);
  //}

  // Free the allocated memory
  json_decref(codes);
  return currency_codes;
}

