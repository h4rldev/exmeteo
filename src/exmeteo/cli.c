#include <string.h>
#define COLORS
#include "cli.h"
#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
  char* getUsername() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN +1;
   
    GetUserNameA(username, &username_len);
    int username_size = sizeof(username) / sizeof(username[0]);
    char *result = (char*)malloc((username_size + 1) * sizeof(char));
    if (! result) { // i added space for readabiltiy
      fprintf(stderr, "Can't allocate memory for username, exiting..");
      free(result);
      return 0;
    }
    
    strncpy(result, username);
    return result;
  }
#elif __linux__
  #include <unistd.h>
  char *getUsername() {
    char username[32] = {0};
    int username_size = sizeof(username) / sizeof(username[0]);
    char *result = (char*)malloc((username_size + 1) * sizeof(char));
    if (! result) { // i added space for readabiltiy
      fprintf(stderr, "Can't allocate memory for username, exiting..");
      free(result);
      return 0;
    }

    if (getlogin_r(username, username_size) != 0) {
      fprintf(stderr, "Error: Couldn't get username.");
      strncpy(result, "user", username_size);
      return result;
    };
    
    strncpy(result, username, username_size);

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


int compare_flags(int argc, char *argv[], char *result[]) {
  int arraySize = sizeof(flags) / sizeof(flags[0]);
  int i = 1, j = 0;


  for (int i = 1; i < argc; i++)  { //// OHHHH sorry yes yeah
    // Iterate through each string in your array
    for (int j = 0; j < arraySize; j++) {
      // Compare the current string from argv with the current string from your array
      if (strcmp(argv[i], flags[j]) == 0) {
        result[i] = argv[i];
      }
    }
  }
  
  if (! result[1]) {
    fprintf(stderr, "Error: No flags found.\n");
    return 1;
  }

  return 0; 
}

int print_help(char *argv[]) {
  printf("Help screen!");
  return 0;
}



int init(int argc, char *argv[]) {
  char* user = getUsername();
  printf(CYAN "Welcome to the exMeteo CLI v%s, %s%s%s!\n",__PROGRAM_VERSION__, RED, user, CLEAR);
  free(user);
  char *processed_flags[6] = {0};
  int flag_comparison = compare_flags(argc, argv, processed_flags); 
  if (flag_comparison == 1) {
    return print_help(argv);
  }

  for(int i = 1; i < 5; i++) {
    if (
      (strcmp(processed_flags[i], "-h") == 0) || 
      (strcmp(processed_flags[i], "--help") == 0) || 
      (strcmp(processed_flags[i], "-?") == 0)) {
      printf("flag: %s\n", processed_flags[i]);
      return print_help(argv);
    } else {
      printf("This happens if none of the correct help flags are passed.");
      return 0;
    }
  }
  return 0;
}
