#include "functions.h"

int handleNumber(char *opt) {
  char *endptr;
  long num = strtol(opt, &endptr, 10);

  if (*endptr != '\0') {
    return 0;
  }

  if (num < 0) {
    return 0;
  }
  
  return 1;
}

int handleString(char *opt) {
  if (opt != NULL) {
    if (strlen(opt) == 0) {
      return 0;
    };
  }
  return 1;
}