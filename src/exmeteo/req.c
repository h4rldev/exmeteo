#include "req.h"
#include "color.h"

/*
 * struct MemoryStruct
 * char *memory;
 * size_t size;
 *
 * The struct that allows allocation of memory to allow getting response.
 */

struct MemoryStruct {
  char *memory;
  size_t size;
};

/*
 * static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
 * void *userp)
 *
 * Reallocates and writes HTTP response to memory.
 * (No I do not know how it works, it's magic code from the CURL devs.)
 */

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr) {
    /* out of memory! */
    fprintf(stderr, RED "!%s Not enough memory (realloc returned NULL)\n",
            CLEAR);
    free(ptr);
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

/*
 * char *req(const char *url)
 *
 * Uses libcurl and allocates the response of a url to a char *, and returns it.
 */

char *req(const char *url) {
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
    free(chunk.memory);
    return 0;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "exMeteo @ libcurl-agent/1.0");

  response = curl_easy_perform(curl);
  if (response != CURLE_OK) {
    fprintf(stderr, RED "!%s curl_easy_perform() failed: %s\n", CLEAR,
            curl_easy_strerror(response));
    free(chunk.memory);
    return 0;
  }
  // printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
  resp = (char *)malloc((chunk.size + 1));
  if (!resp) {
    fprintf(stderr, RED "!%s Can't allocate memory for response, exiting..",
            CLEAR);
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
