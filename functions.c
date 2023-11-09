#include "functions.h"

// Entradas: Char* (string), int* número de celdas
// Salidas: bool
// Descripción: Recibe el string de opt y verifica que sea un número válido y lo guarda en el puntero
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

// Entradas: Char* (string)
// Salidas: bool
// Descripción: Recibe el string de opt y verifica que sea un nombre de archivo válido
int handleString(char *opt) {
  if (opt != NULL) {
    if (strlen(opt) == 0) {
      return 0;
    };
  }
  return 1;
}

int fileExists(char *fname) {
  if (access(fname, F_OK) == 0) {
    return 1;
  }
  return 0;
}
