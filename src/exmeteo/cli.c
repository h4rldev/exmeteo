#include "cli.h"
#include "parse.h"
#include <curl/curl.h>
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
  int getTerminalWidth(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwSize.X;
  }
#elif __linux__
  #include <unistd.h>
  #include <sys/ioctl.h>
  char *getUsername() {
    char username[32] = {0};
    int username_size = sizeof(username) / sizeof(username[0]);
    char *result = (char*)malloc((username_size + 1));
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
  int getTerminalWidth(void) {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
  }
#else
  #error "Unsupported Operating System, use a normal os, loser"
#endif

int compare_flag(char **argv, const char *flag_list[], int flags_len, int level) {

  for (int i = 0; i < flags_len; i++)  {
    if (strcmp(argv[level], flag_list[i]) == 0) {
      return i;
    }
  }
  printf(RED "!%s Unknown command, printing help text..\n\n", CLEAR);
  return 1;
}

int print_line(char* message) {
  int terminal_width = getTerminalWidth();
  for (int i = 0; i < terminal_width; i++) {
    printf("%s", message);
  }
  printf("\n");
  return 0;
}

int info_for_flag(
  char* provided_flags[], 
  int flag_amount,
  char *flags_color,
  char *info,
  char *info_color
) {
  printf("%s%s", flags_color, provided_flags[0]);
  for (int i = 1; i < flag_amount; i++) {
    printf(CLEAR " | %s%s" CLEAR, flags_color, provided_flags[i]);
  }
  printf("\n");
  printf("%s%s\n" CLEAR, info_color, info);
  return 0;
}

int print_help(char ***argv) {
  printf( //Show application & version. 
    GREEN "%s%s v%s%s%s - %sHELP\n" CLEAR,
    __NAME__, CLEAR, CYAN, __PROGRAM_VERSION__, CLEAR, GREEN
  );

  printf( //Usage.
    RED "USAGE: %s%s%s [FLAGS] [OPTIONS] [VALUE] \n", 
    CYAN, *argv[0], CLEAR
  );
  
  print_line("-");
   
  char* help[3]     = {"--help", "-h",};
  char* version[2]  = {"--version", "-v"};
  char* currency[5] = {"--currency", "-c"};
 
  info_for_flag(help, 3, GREEN, "Prints this message.", YELLOW);
  info_for_flag(version, 2, GREEN, "Prints program version.", YELLOW);
  info_for_flag(currency, 5, GREEN, "Convert one currency to another", YELLOW);
  
  print_line("-");

  printf("Made with %s❤%s by %s%s\n" CLEAR, RED, CLEAR, BLUE, __AUTHOR__);

  return 0;
}


int print_version(void) {
  printf(GREEN "%s%s v%s%s\n" CLEAR, __NAME__, CLEAR, CYAN, __PROGRAM_VERSION__);
  return 0;
}

int get_codes(int argc, char **argv) {
    char *api_key = "966eb565013e92b110e1cf0d";
    char ***array = currency__get_codes(api_key, "cache.json");
    printf("%s \n", array[0][1]);
    free_2D_string_array(array, 162);
    return 0;
}

int init(int argc, char *argv[]) {
  char *user = getUsername();
  
  const char* flags[8] = {
    "--help", "-h",     // 0 - 1 help
    "--version", "-v",   // 2 - 3 version 
    "--currency", "-c", // 4 - 5 currency-converter
    "--weather", "-w"   // 6 - 7 weather
  };

  const char* subflags[2] = {
    "--list", "-l"      // list weather / currencies [USELESS AS OF NOW]
  };


  int flag = compare_flag(argv, flags, 8, 1);
  switch (flag) {
    case 0:
    case 1:
      print_help(&argv);
      break;
    case 2:
    case 3:
      print_version();
      break;
    case 4:
    case 5:
      get_codes(argc, argv);
      break;
  };

  free(user);
  return 0;
}
