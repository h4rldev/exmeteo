#include "parse.h"
#include "cache.h"
#include "req.h"
#include "file.h"
#include "color.h"

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
    array[i][0] = (char *)malloc(sub_item0_str_size);
    //printf("Allocating currency_codes[%d][1]..", i);
    array[i][1] = (char *)malloc(sub_item1_str_size);


    strncpy(array[i][0], sub_item0_str, sub_item0_str_size);    
    strncpy(array[i][1], sub_item1_str, sub_item1_str_size);
  }
  
  json_decref(json);
  return array;
}

json_t *get_content_from_element(const char *url, char *element) {

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


char ***currency__get_codes(const char *api_key, const char *filename) {
  char *path = get_path(filename);

  char *url_fmt = "https://v6.exchangerate-api.com/v6/%s/codes";

  int fmt_count = count_instances(url_fmt, "%s"); 
  size_t url_len = (size_t)(strlen(url_fmt) - fmt_count) + strlen(api_key) + 1;
  char url[url_len];

  snprintf(url, url_len, url_fmt, api_key);

  if (file_exists(path)) {
    json_t *codes = read_from_cache(filename);
    if (! codes || ! json_is_array(codes)) {
      fprintf(stderr, RED "!%s Cached Codes is supposed to exist / supposed to be an array, returning 0..\n", CLEAR);

      return 0;
    }

    free(path);

    return get_2D_array_from_json(codes);
  }

  json_t *codes = get_content_from_element(url, "supported_codes");

  if (! codes || ! json_is_array(codes)) {
    fprintf(stderr, RED "!%s Codes is supposed to exist / supposed to be an array, returning 0..\n", CLEAR);
    free(path);

    return 0;
  }

  if(write_to_json(filename, codes, false) == 1) {
    fprintf(stderr, RED "!%s Can't write to json for some reason, does cache exist?.\n", CLEAR);
    free(path);

    return 0;
  }
  free(path);

  return get_2D_array_from_json(codes);
}

float get_conversion_rate(const char *cur1, const char *cur2, const char* api_key) {
  char *url_fmt = "https://v6.exchangerate-api.com/v6/%s/pair/%s/%s";
  size_t url_len = (size_t)strlen(url_fmt) + strlen(api_key) + 1;

  char url[url_len];
  snprintf(url, url_len, url_fmt, cur1, cur2, api_key);
  
  json_t *conversion_rate = get_content_from_element(url, "conversion_rate");
  float value = json_real_value(conversion_rate);
  
  if (isnan(value)) {
    fprintf(stderr, "Failed to retrieve float value\n");
    json_decref(conversion_rate);
    return 1;
  }
  
  json_decref(conversion_rate);

  return value;
}


