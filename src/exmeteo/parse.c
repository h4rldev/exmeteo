#include "parse.h"
#include <cjson/cJSON.h>

cJSON *currency__get_json_value(char* api_key, char* value) {
  char url[80];
  char *codes_response;
  snprintf(url, 80, "https://v6.exchangerate-api.com/v6/%s/codes", api_key);

  printf("url = %s\n", url);
  
  char *response = req(url);
  printf("response: %s \n", response);
  size_t response_length = (size_t)strlen(response);
  printf("response length: %lu \n", response_length);
  
  cJSON *json = cJSON_ParseWithLength(response, response_length);
  if (! json) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != 0) {
      fprintf(stderr, RED "!%s Error while parsing JSON: %s\n", CLEAR, error_ptr);
    }
    return 0;
  }
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value);
  free(response);
  return json_value;
}

void free_2D_string_array(char ***stringList, int totalStrings) {
  for (int i = 0; i < totalStrings; i++) {
    for (int j = 0; j < 2; j++) {
      printf("Freeing array[%d][%d]\n", i, j);
      free(stringList[i][j]);
    }
    printf("Freeing array[%d]", i);
    free(stringList[i]);
  }
  printf("freeing list.");
  free(stringList);
}

char*** currency__get_codes(char *api_key) {
  cJSON *codes = currency__get_json_value(api_key, "supported_codes");
  if (!cJSON_IsArray(codes)) {
    fprintf(stderr, RED "!%s Codes is supposed to be an array, returning 0..", CLEAR);
    cJSON_Delete(codes);
    return 0;
  }
  // Determine the size of the JSON array
  int size = cJSON_GetArraySize(codes);
    // Allocate memory for the 2D array
  char ***currency_codes = (char ***)malloc((size * sizeof(char **)));
    // Iterate over the JSON array and store the strings in the 2D array
  for (int i = 0; i < size; i++) {
    cJSON *item = cJSON_GetArrayItem(codes, i);
    cJSON *code = cJSON_GetArrayItem(item, 0);
    cJSON *name = cJSON_GetArrayItem(item, 1);
    
    char *code_str = cJSON_GetStringValue(code);
    char *name_str = cJSON_GetStringValue(name);

    int code_size = (strlen(code_str) + 1);
    int name_size = (strlen(name_str) + 1);
    
    currency_codes[i] = (char **)malloc((2 * sizeof(char *)));
    printf("Allocating currency_codes[%d][0]..", i);
    currency_codes[i][0] = (char *)malloc(code_size); // +1 for the null terminator
    printf("Allocating currency_codes[%d][1]..", i);
    currency_codes[i][1] = (char *)malloc(name_size); // +1 for the null terminator


    strncpy(currency_codes[i][0], code_str, code_size);
    strncpy(currency_codes[i][1], name_str, name_size);
  }

  // Print the 2D array to verify
  // for (int i = 0; i < size; i++) {
  //  printf("[\"%s\",\"%s\"]\n", currency_codes[i][0], currency_codes[i][1]);
  //}

  // Free the allocated memory
  cJSON_Delete(codes); 
  return currency_codes;
}

