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

char* req(const char* url) {
  CURL *curl;
  char *resp; 
  CURLcode response;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);  
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();

  if (!curl) {
    fprintf(stderr, RED "!%s curl couldn't initialize, exiting..", CLEAR);
    return 0;
  }
    
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  response = curl_easy_perform(curl);
  if(response != CURLE_OK) {
    fprintf(stderr, RED "!%s curl_easy_perform() failed: %s\n", CLEAR, curl_easy_strerror(response));
    return 0; 
  }
  //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
  resp = (char*)malloc((chunk.size + 1));
  if (!resp) {
    fprintf(stderr, "Can't allocate memory for response, exiting..");
    free(resp);
    return 0;
  }

  strncpy(resp, chunk.memory, chunk.size);
  resp[chunk.size] = '\0';
  free(chunk.memory);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return resp;
}
