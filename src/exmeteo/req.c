#include "req.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    fprintf(stderr, RED "!%s Not enough memory (realloc returned NULL)\n", CLEAR);
    free(ptr);
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}



int req(char* url, char* resp) {
  CURL *curl;
  CURLcode response;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);  
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    response = curl_easy_perform(curl);

    
    if(response != CURLE_OK) {
      fprintf(stderr, RED "!%s curl_easy_perform() failed: %s\n", CLEAR, curl_easy_strerror(response));
    } else {
      printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
      strncpy(resp, chunk.memory, chunk.size);
    }
    curl_easy_cleanup(curl);
    free(chunk.memory);
  }
  curl_global_cleanup();
  return 0;
}
