#include "parse.h"

char *currency__get_json_value(char* api_key, char* value) {
  char url[80];
  char *codes_response;
  snprintf(url, 80, "https://v6.exchangerate-api.com/v6/%s/codes", api_key);

  printf("url = %s\n", url);
  
  char *response = req(url);
  size_t response_length = (size_t)strlen(response);
  
  cJSON *json = cJSON_ParseWithLength(response, response_length);
  if (! json) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != 0) {
      fprintf(stderr, RED "!%s Error while parsing JSON: %s\n", CLEAR, error_ptr);
    }
    return 0;
  }
  cJSON *codes = cJSON_GetObjectItemCaseSensitive(json, value);
 
  char *printed_json = cJSON_Print(codes);
  int printed_json_len = strlen(printed_json);
  
  codes_response = (char*)malloc((printed_json_len +1));
  if (!codes_response) {
    fprintf(stderr, RED "!%s Error while allocating to memory.\n", CLEAR);
    free(codes_response);
    return 0;
  }
  
  strncpy(codes_response, printed_json, printed_json_len);
  
  free(response);
  cJSON_free(printed_json);
  cJSON_Delete(json);
  
  return codes_response;
}

