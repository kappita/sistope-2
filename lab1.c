#include <getopt.h>
#include "functions.h"

int main(int argc, char *argv[]) {
  int option, cells;
  int gotIn = 0, gotOut = 0, gotCells = 0;
  char *inPath, *outPath;
  int console = 0;
  FILE *in, *out;
  // Lee el getopt y verifica si cada opción es válida
  while((option = getopt(argc, argv, "N:i:o:D")) != -1) {
    switch (option) {
      case 'N':
        if (!handleNumber(optarg, &cells)) {
          return 1;
        }
        gotCells = 1;
        break;

      case 'i':
        if (!handleString(optarg)) {
          return 1;
        }
        gotIn = 1;
        inPath = optarg;
        break;

      case 'c':
        if (handleString(optarg)) {
          return 1;
        };
        gotOut = 1;
        outPath = optarg;
        break;

      case 'D':
        console = 1;
        break;
    } 
  }
  // En caso de que no se haya ingresado información necesaria
  if (gotCells == 0 || gotIn == 0) {
    printf("Argumentos insuficientes\n");
    return 1;
  }

  // Define un archivo genérico de salida en caso de que no se ingrese
  // nombre
  in = fopen(inPath, "r");
  if (gotOut == 0) {
    out = fopen("resultados.txt", "w");
  } else {
    out = fopen(outPath, "w");
  }
// Crea el material y simula los impactos, luego escribe el archivo
// E imprime por consola si se solicitó.
  material* mat = createMaterial(cells);
  if (!simulateImpacts(in, mat)) {
    printf("El archivo es inválido\n");
    return 1;
  };
  writeEnergy(out, mat);
  if (console) {
    printEnergy(mat);
  }
  return 0;
}