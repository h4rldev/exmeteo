#include "parse.h"
#include "req.h"
#include "cache.h"
#include "file.h"

int count_instances(const char *str, const char *substr) {
    int count = 0;
  char *pos = strstr(str, substr);
    while (pos!= NULL) {
        count++;
        pos = strstr(pos + strlen(substr), substr);
    }
    return count;
}

void free_2D_string_array(char ***string_list, int total_strings) {
  for (int i = 0; i < total_strings; i++) {
    for (int j = 0; j < 2; j++) {
      //printf("Freeing array[%d][%d]\n", i, j);
      free(string_list[i][j]);
    }
    //printf("Freeing array[%d]", i);
    free(string_list[i]);
  }
  //printf("freeing list.");
  free(string_list);
}


char ***get_2D_array_from_json(json_t *json) {
  int size = json_array_size(json);
  // Allocate memory for the 2D array
  char ***array = (char ***)malloc((size * sizeof(char **)));
  if (!array) {
    fprintf(stderr, RED "!%s Can't allocate memory for 2D array, returning 0..\n", CLEAR);
    free(array);
    return 0;
  }
    // Iterate over the JSON array and store the strings in the 2D array
  for (int i = 0; i < size; i++) {
    json_t *item = json_array_get(json, i);
    json_t *sub_item0 = json_array_get(item, 0);
    json_t *sub_item1 = json_array_get(item, 1);
    
    const char *sub_item0_str = json_string_value(sub_item0);
    const char *sub_item1_str = json_string_value(sub_item1);

    int sub_item0_str_size = (strlen(sub_item0_str) + 1);
    int sub_item1_str_size = (strlen(sub_item1_str) + 1);
    
    array[i] = (char **)malloc((2 * sizeof(char *)));
    //printf("Allocating currency_codes[%d][0]..", i);
    array[i][0] = (char *)malloc(sub_item0_str_size); // +1 for the null terminator
    //printf("Allocating currency_codes[%d][1]..", i);
    array[i][1] = (char *)malloc(sub_item1_str_size); // +1 for the null terminator


    strncpy(array[i][0], sub_item0_str, sub_item0_str_size);    
    strncpy(array[i][1], sub_item1_str, sub_item1_str_size);
  }
  
  json_decref(json);
  return array;
}

json_t *currency__get_content_from_element(const char* url_fmt, const char* api_key, char* element) {
  int fmt_count = count_instances(url_fmt, "%s"); 
  size_t url_len = (size_t)(strlen(url_fmt) - fmt_count) + strlen(api_key) + 1;
  char url[url_len];

  char *codes_response;

  snprintf(url, url_len, url_fmt, api_key);

  char *response = req(url);
  size_t response_length = (size_t)strlen(response) + 1;
  
  json_t *json = json_loads(response, response_length, 0); // this leaks 381 bytes.
  if (! json) {
    fprintf(stderr, RED "!%s Error while parsing JSON.\n", CLEAR);
    return 0;
  }
  
  json_t *selected_element = json_object_get(json, element);
  if (selected_element != 0) {
      json_incref(selected_element);
  }
  json_decref(json);

  free(response);
  return selected_element;
}

char*** currency__get_codes(const char *api_key, const char *filename_to_write) {
  char *path = get_path(filename_to_write);

  json_t *codes = currency__get_content_from_element("https://v6.exchangerate-api.com/v6/%s/codes", api_key, "supported_codes");
  
  if (! codes || ! json_is_array(codes)) {
    fprintf(stderr, RED "!%s Codes is supposed to exist / suppose to be an array, returning 0..\n", CLEAR);
    return 0;
  }
  
  return get_2D_array_from_json(codes);
}


