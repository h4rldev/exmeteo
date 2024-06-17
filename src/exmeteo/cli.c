#include "cli.h"
#include "color.h"
#include "metadata.h"
#include "parse.h"

#ifdef _WIN32
#include <windows.h>
int get_terminal_width(void) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  return csbi.dwSize.X;
}
#elif __linux__
#include <sys/ioctl.h>
#include <unistd.h>
int get_terminal_width(void) {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  return w.ws_col;
}
#else
#error "Unsupported Operating System, use a normal os, loser"
#endif

/*
 * struct Pair
 * char* first;
 * char* second;
 *
 *
 * The struct needed for split_array to work.
 */

typedef struct {
  char *first;
  char *second;
} Pair;

/*
 * void split_array(char **array, int size, Pair* pairs)
 *
 * Splits an array in 2, populating the Pairs and allowing you to split an array
 * of 3 or more into sets of 2 or 1.
 */

void split_array(char **array, int size, Pair *pairs) {
  int index = 0;
  for (int i = 0; i < size; i += 2) {
    pairs[index].first = array[i];
    pairs[index].second = array[i + 1];
    index++;
  }
}

/*
 * int compare_flag(int argc, char **argv, char *command_type, char **flag_list,
 * int flags_len, int level)
 *
 * Compares the flags with argv and returns an int based on that value, being
 * the index of that array where the flags are.
 *
 * Usage:
 * char *flags[2] = { "-h", "--help" };
 * int selected_flag = compare_flag(argc, argv, "command", flags, 2, 1);
 *
 * if (selected_flag == 0 || selected_flag == 1) {
 *    printf("This is help");
 * }
 */

int compare_flag(int argc, char **argv, char *command_type, char **flag_list,
                 int flags_len, int level) {
  if (level >= argc) {
    fprintf(stderr, RED "!%s Unknown %s, printing help text..\n\n", CLEAR,
            command_type);
    return 1;
  }

  for (int i = 0; i < flags_len; i++) {
    if (strcmp(argv[level], flag_list[i]) == 0) {
      return i;
    }
  }

  fprintf(stderr, RED "!%s Unknown %s, printing help text..\n\n", CLEAR,
          command_type);
  return 1;
}

/*
 * int print_line(char* message)
 *
 * Prints a message over and over accross the entire terminal buffer.
 *
 * Usage:
 *
 * print_line("-");
 */

int print_line(char *message) {
  int terminal_width = get_terminal_width();
  for (int i = 0; i < terminal_width; i++) {
    printf("%s", message);
  }
  printf("\n");
  return 0;
}

/*
 * int info_for_flag(const char **provided_flags, int flag_amount, const char
 * *flags_color, const char *info, const char *info_color)
 *
 * Prints out a pretty little section of each flag seperated with | followed by
 * a newline and then the info, in colors of choice.
 *
 * Usage:
 * const char *flags[2] = { "-h", "--help" };
 * char *info = "This prints out help!";
 *
 * int info_for_flag(flags, 2, YELLOW, info, GREEN);
 */

int info_for_flag(const char **provided_flags, int flag_amount,
                  const char *flags_color, const char *info,
                  const char *info_color) {
  printf("%s%s", flags_color, provided_flags[0]);
  for (int i = 1; i < flag_amount; i++) {
    printf(CLEAR " | %s%s" CLEAR, flags_color, provided_flags[i]);
  }
  printf("\n");
  printf("%s%s\n" CLEAR, info_color, info);
  return 0;
}

/*
 * int print_sub_help(char ***argv, char **command, int flag_size, char **flags,
 * const char **help_texts)
 *
 * Prints out help for sub_commands, a small adjustment of the original help
 * command.
 */

int print_sub_help(char ***argv, char **command, int flag_size, char **flags,
                   const char **help_texts) {
  Pair subflag_pairs[flag_size / 2];

  split_array(flags, flag_size, subflag_pairs);

  printf( // Prints the executable, command and a large HELP.
      GREEN "%s %s%s - %sHELP\n" CLEAR, *argv[0], *command, CLEAR, GREEN);

  printf( // Usage.
      RED "USAGE: %s%s %s%s [FLAGS] [VALUE]..[VALUE] \n", CYAN, *argv[0],
      *command, CLEAR);

  print_line("-");

  // Splits and gives info about the sub_flags
  for (int i = 0; i < flag_size / 2; i++) {
    const char *split_flags[2] = {subflag_pairs[i].first,
                                  subflag_pairs[i].second};
    info_for_flag(split_flags, 2, GREEN, help_texts[i], YELLOW);
  }

  print_line("-");

  printf("Made with %s❤%s by %s%s\n" CLEAR, RED, CLEAR, BLUE, __AUTHOR__);

  return 0;
}

/*
 * int print_help(char ***argv, int flag_size, char** flags, const char**
 help_texts)
 *
 * Prints help information based on flags and help_texts provided through
 params.
 *
 * Usage:
 * char* flags[8] = {
 *   "--help", "-h",     // 0-1: help
 *   "--version", "-v",  // 2-3: version
 *   "--currency", "-c", // 4-5: currency
 *   "--weather", "-w"   // 6-7: weather
 * };

 * const char* help_texts[4] = {
 *   "Prints this message.",
 *   "Prints program version",
 *   "Currency conversion commands",
 *   "Get the weather",
 * };
 *
 * print_help(&argv, 8, flags, help_texts);
 */

int print_help(char ***argv, int flag_size, char **flags,
               const char **help_texts) {
  Pair flag_pairs[flag_size / 2];

  split_array(flags, flag_size, flag_pairs);

  printf( // Show application & version.
      GREEN "%s%s v%s%s%s - %sHELP\n" CLEAR, __NAME__, CLEAR, CYAN,
      __PROGRAM_VERSION__, CLEAR, GREEN);

  printf( // Usage.
      RED "USAGE: %s%s%s [FLAGS] [OPTIONS] [VALUE] \n", CYAN, *argv[0], CLEAR);

  print_line("-");

  for (int i = 0; i < flag_size / 2; i++) {
    const char *split_flags[2] = {flag_pairs[i].first, flag_pairs[i].second};
    info_for_flag(split_flags, 2, GREEN, help_texts[i], YELLOW);
  }

  print_line("-");

  printf("Made with %s❤%s by %s%s\n" CLEAR, RED, CLEAR, BLUE, __AUTHOR__);
  return 0;
}

/*
 * int print_version(void)
 *
 * Pretty prints the version info of the program, gets it's data from metadata.h
 */

int print_version(void) {
  printf(GREEN "%s%s v%s%s\n" CLEAR, __NAME__, CLEAR, CYAN,
         __PROGRAM_VERSION__);
  return 0;
}

/*
 * int convert(int argc, char **argv)
 *
 * A subfunction to currency, handles conversion.
 */

int convert(char *cur1, char *cur2, char *api_key, float amount) {
  float conversion_rate = currency__get_conversion_rate(cur1, cur2, api_key);
  if (!conversion_rate) {
    fprintf(stderr, RED "!%s Failed to get conversion rate!", CLEAR);
    return 1;
  }
  float calculated_rate = amount * conversion_rate;

  // Check if amount is equal to an integer cast of amount and print it as int
  // incase it is.
  if (amount == (int)amount) {
    printf(BLUE ">%s %d %s is equal to %f %s \n", CLEAR, (int)amount, cur1,
           calculated_rate, cur2);
    return 0;
  }

  printf(BLUE ">%s %f %s is equal to %f %s \n", CLEAR, amount, cur1,
         calculated_rate, cur2);
  return 0;
}

/*
 * int currency(int argc, char **argv)
 *
 * Handles currency and it's subcommands.
 */

int currency(int argc, char **argv) {
  char *api_key = "966eb565013e92b110e1cf0d";
  char ***codes = currency__get_codes(api_key, "cache.json");
  char *subflags[8] = {
      "--help",    "-h", // 0-1: help
      "--list",    "-l", // 2-3: list
      "--convert", "-c", // 4-5: convert currency
      "--get",     "-g"  // 6-7: get conversion rate
  };

  const char *help_texts[4] = {
      "Prints this message.",
      "List all supported currencies (Uses cache if available otherwise gets "
      "from API)",
      "Convert one currency to another, uses currency codes.",
      "Get conversion rate of currency",
  };

  int sub_flag = compare_flag(argc, argv, "subcommand", subflags, 8, 2);
  switch (sub_flag) {
  // --help | -h | ?
  case 0:
  case 1:
    print_sub_help(&argv, &argv[1], 8, subflags, help_texts);
    break;

  // --list | -l
  case 2:
  case 3:
    printf("Current currencies are: \n");
    for (int i = 0; i < 162; i++) {
      printf(BLUE ">%s %sCode%s: %s | %sName%s: %s\n" CLEAR, CLEAR, GREEN,
             CLEAR, codes[i][0], RED, CLEAR, codes[i][1]);
    }
    break;

  // --convert | -c
  case 4:
  case 5:
    if (argc <= 5) {
      fprintf(stderr,
              RED "!%s You need to specify currency code or value!\n "
                  "Example:%s %s %s %s USD EUR 1\n" CLEAR,
              CLEAR, CYAN, argv[0], argv[1], argv[2]);
      break;
    }

    convert(argv[3], argv[4], api_key, atof(argv[5]));
    break;

  // --get | -g
  case 6:
  case 7:
    if (argc <= 4) {
      fprintf(stderr,
              RED "!%s You need to specify currency code!\n Example:%s %s %s "
                  "%s USD EUR\n" CLEAR,
              CLEAR, CYAN, argv[0], argv[1], argv[2]);
      break;
    }

    float conversion_rate =
        currency__get_conversion_rate(argv[3], argv[4], api_key);
    if (!conversion_rate) {
      fprintf(stderr, RED "!%s Failed to get conversion rate!", CLEAR);
      break;
    }

    printf(BLUE ">%s The conversion rate of %s%s%s to %s%s%s is: %s%f\n" CLEAR,
           CLEAR, GREEN, argv[3], CLEAR, GREEN, argv[4], CLEAR, CYAN,
           conversion_rate);
    break;

  default:
    fprintf(stderr, RED "!%s Somehow got an invalid flag, returning 1.\n",
            CLEAR);
    free_2D_string_array(codes, 162);
    return 1;
  }

  free_2D_string_array(codes, 162);
  return 0;
}

/*
 * int weather(int argc, char **argv)
 *
 * Handles the weather command and it's subcommands.
 */

int weather(int argc, char **argv) {
  char *subflags[24] = {
      "--help",          "-h",   // 0-1:   help
      "--get",           "-g",   // 2-3:   get weather in standard format.
      "--detailed",      "-d",   // 4-5:   get detailed weather data.
      "--temp",          "-tp",  // 6-7:   get temperature for an area.
      "--moon-phase",    "-mp",  // 8-9:   get moon phase
      "--moon-day",      "-md",  // 10-11: get moon day
      "--humidity",      "-hu",  // 12-13: get humidity
      "--wind",          "-w",   // 14-15: get wind
      "--precipitation", "-prc", // 16-17: get precipitation (mm/3 hours)
      "--pressure",      "-pr",  // 18-19: get pressure in hPa
      "--uv-index",      "-uv",  // 12-21: get UV index.
      "--time",          "-tm",  // 22-23: get time diagnostics
  };

  const char *help_texts[12] = {
      "Prints this message.",
      "Get current weather in an area.",
      "Get detailed weather information in an area.",
      "Get current temperature and what the temperature feels like in an area.",
      "Get current moon phase.",
      "Get current moon day.",
      "Get the humidity in an area.",
      "Get wind and wind speed in an area.",
      "Get precipitation in an area.",
      "Get pressure in an area.",
      "Get the UV index of an area.",
      "Get time data such as sunrise in an area."};

  char *format = {0};
  char *fmt_string = {0};
  char *weather_data = {0};
  int sub_flag = compare_flag(argc, argv, "subcommand", subflags, 24, 2);

  if (sub_flag == 0 || sub_flag == 1) {
    print_sub_help(&argv, &argv[1], 24, subflags, help_texts);
    return 0;
  }

  if (argc <= 3) {
    fprintf(stderr,
            RED "!%s You need to specify a place! \nExample:%s %s %s %s "
                "karlstad\n" CLEAR,
            CLEAR, CYAN, argv[0], argv[1], argv[2]);
    return 0;
  }

  // --get | -g
  if (sub_flag == 2 || sub_flag == 3) {
    weather_data = weather__get_weather_data(argv[3], false, true);
    if (!weather_data) {
      fprintf(stderr, RED "!%s Failed to get weather data!", CLEAR);
      free(weather_data);
      return 1;
    }
    puts(weather_data);

    free(weather_data);
    return 0;
  }

  // --detailed | -d
  if (sub_flag == 4 || sub_flag == 5) {
    weather_data = weather__get_weather_data(argv[3], true, true);
    if (!weather_data) {
      fprintf(stderr, RED "!%s Failed to get weather data!", CLEAR);
      free(weather_data);
      return 1;
    }
    puts(weather_data);

    free(weather_data);
    return 0;
  }

  switch (sub_flag) {
  // --temp | tp
  case 6:
  case 7:
    format = "%t%20%f";
    break;

  // --moon-phase | -mp
  case 8:
  case 9:
    format = "%m";
    fmt_string = "Current moon phase:";
    break;

  // --moon-day | -md
  case 10:
  case 11:
    format = "%M";
    fmt_string = "Current moon day:";
    break;
  // --humidity | -hu
  case 12:
  case 13:
    format = "%h";
    fmt_string = "Current humidity:";
    break;

  // --wind | -w
  case 14:
  case 15:
    format = "%w";
    fmt_string = "Current wind:";
    break;

  // --percipitation | -prc
  case 16:
  case 17:
    format = "%p";
    fmt_string = "Current percipitation:";
    break;

  // --pressure | -pr
  case 18:
  case 19:
    format = "%P";
    fmt_string = "Current pressure:";
    break;

  // --uv-index | -uv
  case 20:
  case 21:
    format = "%u";
    fmt_string = "Current UV index:";
    break;

  // --time | -tm
  case 22:
  case 23:
    format = "%D%20%S%20%z%20%s%20%d%20%T%20%Z";
    break;

  default:
    fprintf(stderr, RED "!%s Incorrect flag set?", CLEAR);
    return 1;
  }

  weather_data = weather__get_weather_data_w_format(argv[3], format);
  if (!weather_data) {
    fprintf(stderr, RED "!%s Failed to get weather data!", CLEAR);
    free(weather_data);
    return 1;
  }

  // --temp | tp
  if (sub_flag == 6 || sub_flag == 7) {
    char *temps = strtok(weather_data, " ");

    printf(BLUE ">%s Temperature in %s%s%s: %s\n", CLEAR, CYAN, argv[3], CLEAR,
           temps);
    temps = strtok(0, " ");
    printf(BLUE ">%s Feels like: %s\n", CLEAR, temps);
    return 0;
  }

  // --time | tm
  if (sub_flag == 22 || sub_flag == 23) {
    char *times[7] = {0};
    int index = 1;
    char *token;

    token = strtok(weather_data, " ");
    times[0] = token;
    while (token != 0) {
      token = strtok(NULL, " ");
      times[index] = token;
      index++;
    }

    printf(BLUE ">%s The day dawned / will dawn at: %s\n", CLEAR, times[0]);
    printf(BLUE ">%s The time of sunrise was/is gonna be: %s\n", CLEAR,
           times[1]);
    printf(BLUE
           ">%s The time when the sun is at it's Zenith was/is gonna be: %s\n",
           CLEAR, times[2]);
    printf(BLUE ">%s The time the sun will/was set is: %s\n", CLEAR, times[3]);
    printf(BLUE ">%s The time when it became/will be dusk is: %s\n", CLEAR,
           times[4]);
    printf(BLUE ">%s The current time is: %s\n", CLEAR, times[5]);
    printf(BLUE ">%s The local timezone is: %s\n", CLEAR, times[6]);
    return 0;
  }

  // Everything else that populates fmt_string.
  printf(BLUE ">%s %s %s\n", CLEAR, fmt_string, weather_data);

  free(weather_data);
  return 0;
}

/*
 * int init(int argc, char **argv)
 *
 * Initializes the CLI entirely, handling help, version, currency and weather
 * commands. Has subfunctions
 */

int init(int argc, char **argv) {

  char *flags[8] = {
      "--help",     "-h", // 0-1: help
      "--version",  "-v", // 2-3: version
      "--currency", "-c", // 4-5: currency
      "--weather",  "-w"  // 6-7: weather
  };

  const char *help_texts[4] = {
      "Prints this message.",
      "Prints program version",
      "Currency conversion commands",
      "Get the weather",
  };

  int flag = compare_flag(argc, argv, "command", flags, 8, 1);
  switch (flag) {
  // --help | -h | ?
  case 0:
  case 1:
    print_help(&argv, 8, flags, help_texts);
    break;

  // --version | -v
  case 2:
  case 3:
    print_version();
    break;

  // --currency | -c
  case 4:
  case 5:
    currency(argc, argv);
    break;

  // --weather | -w
  case 6:
  case 7:
    weather(argc, argv);
    break;

  default:
    fprintf(stderr, RED "!%s Somehow got an invalid flag, returning 1.\n",
            CLEAR);
    return 1;
  };

  return 0;
}
