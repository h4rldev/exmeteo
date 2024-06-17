#include "file.h"
#include "cache.h"
#include "color.h"

/*
 * bool dir_exists(const char *path)
 *
 * Returns true if a directory exists, if not returns false.
 *
 * Usage:
 * if (dir_exists("/home/user/")) {
 *   printf("/home/user exists!");
 * }
 */

bool dir_exists(const char *path) {
  if (path == 0) {
    return false;
  }
#ifdef _WIN32
  DWORD res = GetFileAttributesA(path);
  return (res != INVALID_FILE_ATTRIBUTES && (res & FILE_ATTRIBUTE_DIRECTORY));
#else
  struct stat fi;
  if (stat(path, &fi) == 0) {
    return (S_ISDIR(fi.st_mode));
  }
  return false;
#endif
}

/*
 * bool file_exists(const char *path)
 *
 * Returns true if a file exists, if not, returns false.
 *
 * Usage:
 * if (file_exists("/home/user/.cache/exmeteo/cache.json")) {
 *   printf("Cache exists!");
 * }
 */

bool file_exists(const char *path) {
  FILE *file = fopen(path, "r");
  if (file) {
    fclose(file);
    return true; // File exists
  } else {
    return false; // File does not exist
  }
}

/*
 * int create_dir(const char *path)
 *
 * Creates a dir on the system.
 *
 * Usage:
 * create_dir("/home/user/test.txt");
 */

int create_dir(const char *path) {
  int result = 0;
#ifdef _WIN32
  // Windows uses _mkdir instead of mkdir
  result = _mkdir(path);
#else
  // POSIX systems use mkdir
  result = mkdir(path, 0777); // Use 0777 for full permissions; adjust as needed
#endif

  if (result != 0) {
    perror("Failed to create directory");
    return 1;
  }
  return 0;
}

/*
 * char *get_path(const char* filename)
 *
 * Gets the default path to the cache, uses malloc though, so be sure to free
 * it!
 *
 * Usage:
 * char *path = get_path("cache.json");
 * printf("path is %s", path);
 * free(path);
 */

char *get_path(const char *filename) {
  size_t size_of_cache_dir = (size_t)(strlen(CACHE_DIR) + strlen(HOME)) + 1;
  char cache_dir[size_of_cache_dir];

  snprintf(cache_dir, size_of_cache_dir, "%s%s", HOME, CACHE_DIR);

  if (!dir_exists(cache_dir)) {
    create_dir(cache_dir);
  }

  size_t completed_path_size =
      (size_t)(strlen(cache_dir) + strlen(filename)) + 1;
  char completed_path[completed_path_size];

  snprintf(completed_path, completed_path_size, "%s%s", cache_dir, filename);

  char *path = (char *)malloc(completed_path_size);
  if (!path) {
    fprintf(stderr, RED "!%s Error while allocating path.\n", CLEAR);
    return 0;
  }

  strncpy(path, completed_path, completed_path_size);

  return path;
}
