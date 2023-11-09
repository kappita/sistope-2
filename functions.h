#include "structs.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>




// Entradas: Char* (string), int* número de celdas
// Salidas: bool
// Descripción: Recibe el string de opt y verifica que sea un número válido y lo guarda en el puntero
int handleNumber(char *opt);
// Entradas: Char* (string)
// Salidas: bool
// Descripción: Recibe el string de opt y verifica que sea un nombre de archivo válido
int handleString(char *opt);

