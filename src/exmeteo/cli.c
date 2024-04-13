#include "cli.h"
#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
  void getUsername(char result[]) {
    char username[1024];
    DWORD username_len = 1024;
    GetUserNameA(username, &username_len);
    result = username;
  }
#elif __linux__
  #include <unistd.h>
  void getUsername(char *result) {
    char username[1024];
    if (getlogin_r(username, sizeof(username)) == 0) {
      result = username;
    } else {
      fprintf(stderr, "Error: Couldn't get username.");
      result = "user";
    }
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


void compare_flags(int argc, char **argv, char *result[]) {
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
}



int init(int argc, char **argv) {
  char* user;
  getUsername(user);
  printf("Welcome to the EXMeteo CLI %s", user);

  char *processed_flags[5] = {NULL};
  compare_flags(argc, argv, processed_flags);
  for(int i = 1; i < 5; i++) {
    if (processed_flags[i] != NULL) {
       printf("valid flags parsed: %s\n", processed_flags[i]);
    }
  }
  return 0;
}
