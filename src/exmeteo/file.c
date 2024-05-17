#include "file.h"
#include "cache.h"
#include "color.h"

bool dir_exists(const char* dirname) {
    if (dirname == 0) { return false; }
    #ifdef _WIN32
        DWORD res = GetFileAttributesA(folder);
        return (res!= INVALID_FILE_ATTRIBUTES && (res & FILE_ATTRIBUTE_DIRECTORY));
    #else
        struct stat fi;
        if (stat(dirname, &fi) == 0) {
            return (S_ISDIR(fi.st_mode));
        }
        return false;
    #endif
}


bool file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true; // File exists
    } else {
        return false; // File does not exist
    }
}

int create_dir(const char* dirname) {
    int result = 0;
#ifdef _WIN32
    // Windows uses _mkdir instead of mkdir
    result = _mkdir(dirname);
#else
    // POSIX systems use mkdir
    result = mkdir(dirname, 0777); // Use 0777 for full permissions; adjust as needed
#endif

    if (result!= 0) {
        perror("Failed to create directory");
        return 1;
    }
    return 0;
}

char *get_path(const char* filename) {
  size_t size_of_cache_dir = (size_t)(strlen(CACHE_DIR) + strlen(HOME)) + 1;
  char cache_dir[size_of_cache_dir]; 
  
  snprintf(cache_dir, size_of_cache_dir, "%s%s", HOME, CACHE_DIR);

  if (!dir_exist(cache_dir)) {
    create_dir(cache_dir); 
  }

  size_t completed_path_size = (size_t)(strlen(cache_dir) + strlen(filename));
  char completed_path[completed_path_size];

  snprintf(completed_path, completed_path_size, "%s%s", cache_dir, filename);

  char* path = (char *)malloc(completed_path_size);
  if (!path) {
    fprintf(stderr, RED "!%s Error while allocating path.\n", CLEAR);
    return 0;
  }
  
  strncpy(path, completed_path, completed_path_size);

  return path;
}
