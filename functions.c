#include "functions.h"

int handleNumber(char *opt) {
  char *endptr;
  long num = strtol(opt, &endptr, 10);

  if (*endptr != '\0') {
    printf("Por favor, ingrese un numero");
    return 0;
  }

  if (num < 0) {
    printf("Por favor ingrese numero no negativo");
    return 0;
  }
  
  return 1;
}

int handleString(char *opt) {
  if (opt != NULL) {
    if (strlen(opt) == 0) {
      printf("Por favor ingrese nombre de archivo");
      return 0;
    };
  }
  return 1;
}



material* createMaterial(int cellCount) {
  material* mat = (material*)malloc(sizeof(material));
  mat->cellCount = cellCount;
  mat->cells = (double*)malloc(sizeof(double) * cellCount);
  return mat;
}

double impactFormula(int cellPos, int impactedCellPos, int cellEnergy, int cellCount) {
  double result = (pow(10, 3) * cellEnergy) /
                  (cellCount * pow(abs(impactedCellPos - cellPos) + 1, 0.5));
  if (result < (pow(10, -3) / cellCount)) {
    return (double)0;
  };
  return result;
}

void impactMaterial(material *mat, int particlePos, int particleEnergy) {
  // Se aplica la fórmula de impacto para cada celda del material
  for (int i = 0; i < mat->cellCount; i++) {
    double impact = impactFormula(i, particlePos, particleEnergy, mat->cellCount);
    mat->cells[i] = mat->cells[i] + impact;
  }
  return;
}

int simulateImpacts(FILE *fp, material *mat) {
  // Se lee el archivo de partículas y se aplica el impacto al material
  // con cada uno
  int particleCount, pos, energy, scanResponse;
  scanResponse = fscanf(fp, "%d", &particleCount);
  if (scanResponse != 1) {
    return 0;
  }

  for (int i = 0; i < particleCount; i++) {
    scanResponse = fscanf(fp, "%d %d", &pos, &energy);
    if (scanResponse != 2) {
      return 0;
    }
    impactMaterial(mat, pos, energy);
  }

  return 1;
}


int getMaxEnergyCellPos(material* mat) {
  double max = 0;
  int pos;
  for (int i = 0; i < mat->cellCount; i++) {
    if (mat->cells[i] > max) {
      max = mat->cells[i];
      pos = i;
    }
  }
  return pos;
}

int getCharAmount(double number) {
  int i = 1;
  while (number >= 10) {
    number = number / 10;
    i++;
  }
  // Se retorna el total de digitos de la parte entera
  return i;
}

void printCell(int cellPos, int energyChars, int cellsChars, double cellEnergy) {
  printf("%i", cellPos);
  int cellChars = getCharAmount(cellPos);
  int cellEnergyChars = getCharAmount(cellEnergy);
  for (int i = 0; i < cellsChars - cellChars + 1; i++) {
    printf(" ");
  }
  for (int i = 0; i < energyChars - cellEnergyChars; i++) {
    printf(" ");
  }
  printf("%.4f |", cellEnergy);
  return;
}

void printCellEnergy(double cellEnergy, double part) {
  while (cellEnergy > part) {
    cellEnergy = cellEnergy - part;
    printf("o");
  }
  printf("\n");
  return;
}

void printEnergy(material* mat) {
  struct winsize w;
  double maxEnergy = mat->cells[getMaxEnergyCellPos(mat)];
  int energyChars = getCharAmount(maxEnergy); 
  int cellsChars = getCharAmount(mat->cellCount);
  ioctl(0, TIOCGWINSZ, &w); // Obtiene información de la consola
  // Define cuánta energía equivale a cada o mostrado por consola
  double part = maxEnergy / (w.ws_col - (8 + energyChars + cellsChars));

  for (int i = 0; i < mat->cellCount; i++) {
    printCell(i, energyChars, cellsChars, mat->cells[i]);
    printCellEnergy(mat->cells[i], part);
  }
  return;
}

void writeCell(FILE *fp, int cellPos, int energyChars, int maxCellsChars, double cellEnergy) {
  fprintf(fp, "%i", cellPos);
  int cellChars = getCharAmount(cellPos);
  int cellEnergyChars = getCharAmount(cellEnergy);
  for (int i = 0; i < maxCellsChars - cellChars + 1; i++) {
    fprintf(fp, " ");
  }
  for (int i = 0; i < energyChars - cellEnergyChars; i++) {
    fprintf(fp, " ");
  }
  fprintf(fp, "%.6f\n", cellEnergy);
  return;
}

void writeEnergy(FILE *fp, material* mat) {
  int maxEnergyPos = getMaxEnergyCellPos(mat);
  double maxEnergy = mat->cells[maxEnergyPos];
  int energyChars = getCharAmount(maxEnergy); 
  int cellsChars = getCharAmount(mat->cellCount);

  writeCell(fp, maxEnergyPos, energyChars, cellsChars, maxEnergy);
  for (int i = 0; i < mat->cellCount; i++) {
    writeCell(fp, i, energyChars, cellsChars, mat->cells[i]);
  }
  fclose(fp);
  return;
}



