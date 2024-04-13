#include "cli.h"
#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
  char* getUsername() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN +1;
   
    GetUserNameA(username, &username_len);

    result = (char*)malloc((strlen(username)) * sizeof(char));
    if (result == NULL) {
      fprintf(stderr, "Can't allocate memory for username, exiting..");
      free(result);
      return NULL;
    }
    
    strncpy(result, username);
    return result;
  }
#elif __linux__
  #include <unistd.h>
  char *getUsername() {
    char username[32];
    char *result = NULL; 
    result = (char*)malloc((strlen(username) + 1) * sizeof(char));
    if (result == NULL) {
      fprintf(stderr, "Can't allocate memory for username, exiting..");
      free(result);
      return NULL;
    }

    if (getlogin_r(username, sizeof(username)) != 0) {
      fprintf(stderr, "Error: Couldn't get username.");
      strncpy(result, "user", 32);
      return result;
    };
    
    strncpy(result, username, 32);

    return result;
  }
#else
  #error "Unsupported Operating System, use a normal os, loser"
#endif


const char* flags[5] = {
  "--help",     // help-flag
  "-?",         // shorterm for help
  "-h",         // alternative shorterm for help
  "--version",  // version-flag
  "-v",         // shorterm for version
};


int compare_flags(int argc, char **argv, char *result[]) {
  int arraySize = sizeof(flags) / sizeof(flags[0]);

  for (int i = 1; i < argc; i++) {
    // Iterate through each string in your array
    for (int j = 0; j < arraySize; j++) {
      // Compare the current string from argv with the current string from your array
      if (strcmp(argv[i], flags[j]) == 0) {
        printf("Found match: %s\n", argv[i]);
        result[i] = argv[i];
      }
    }
  }
  
  if (result[0] == NULL) {
    fprintf(stderr, "Error: No flags found.\n");
    return 1;
  }

  return 0; 
}



int init(int argc, char **argv) {
  char* user = getUsername();
  printf("Welcome to the EXMeteo CLI %s\n", user);
  free(user);
  char *processed_flags[6] = {NULL};
  compare_flags(argc, argv, processed_flags);
  for(int i = 1; i < 5; i++) {
    if (processed_flags[i] != NULL) {
       printf("valid flags parsed: %s\n", processed_flags[i]);
    }
  }
  return 0;
}
