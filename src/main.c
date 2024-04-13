#include "./exmeteo/gui.h"
#include "./exmeteo/cli.h"

int main(int argc, char **argv) { 
  if (strlen(*argv) > 1) {
    init(argc, argv);
  } else {
    test(argc, argv);
  }
  
  return 0;
}


