#include "fbroker.h"

#define BUFFER_MAX 64

#define PIPE_TO_C 0
#define PIPE_TO_P 1

#define PIPE_READ 0
#define PIPE_WRITE 1

#define SUBSET_SIZE 1000

material* createMaterial(int cellCount) {
  material* mat = (material*)malloc(sizeof(material));
  mat->cellCount = cellCount;
  mat->cells = (double*)malloc(sizeof(double) * cellCount);
  for (int i = 0; i < cellCount; i++) {
    mat->cells[i] = 0;
  }
  return mat;
}

int* createSinglePipe() {
  int* newPipe = (int*)malloc(sizeof(int) * 2);
  if (pipe(newPipe) == -1) {
    printf("Error al crear el pipe\n");
    exit(1);
  }
  return newPipe;
}

int** createDualPipe() {
  int** pipes = (int**)malloc(sizeof(int*) * 2);
  for (int j = 0; j < 2; j++) {
    pipes[j] = createSinglePipe();
  }
  return pipes;

}

int*** createPipes(int workerCount) {
  // Se deben crear dos pipes por worker
  int*** pipes = (int***)malloc(sizeof(int**) * workerCount);
  for (int i = 0; i < workerCount; i++) {
    pipes[i] = createDualPipe();
  }

  return pipes;
}


void createWorkers(int workerCount, char* cellCountChar, char* chunkChar, int*** pipes, int* pids) {
  for (int i = 0; i < workerCount; i++) {
    int pid = fork();
    if (pid == -1) {
      printf("Error al crear el proceso hijo\n");
      exit(1);
    }
    if (pid == 0) {
      // Lógica del proceso hijo
      close(pipes[i][PIPE_TO_C][PIPE_WRITE]);
      close(pipes[i][PIPE_TO_P][PIPE_READ]);

      char fdCBuffer[BUFFER_MAX];
      char fdPBuffer[BUFFER_MAX];
      char subsetSizeChar[BUFFER_MAX];

      sprintf(fdCBuffer, "%d", pipes[i][PIPE_TO_C][PIPE_READ]);
      sprintf(fdPBuffer, "%d", pipes[i][PIPE_TO_P][PIPE_WRITE]);
      sprintf(subsetSizeChar, "%d", SUBSET_SIZE);

      char* args[] = {"./worker", cellCountChar, fdCBuffer, fdPBuffer, subsetSizeChar, NULL};
      execv("./worker", args);
      printf("Error ejecutando proceso worker\n");

    } else {
      pids[i] = pid;
      close(pipes[i][PIPE_TO_C][PIPE_READ]);
      close(pipes[i][PIPE_TO_P][PIPE_WRITE]);
    }
  }
}

void sendLineToWorker(int** processPipes, char* line) {
  if (write(processPipes[PIPE_TO_C][PIPE_WRITE], line, BUFFER_MAX * sizeof(char)) < 0) {
    printf("Error enviando linea al worker");
    exit(1);
  }
}

void readReadLines(int* readLines, int** processPipes, int pipeNumber) {
  int i;

  if (read(processPipes[PIPE_TO_P][PIPE_READ], &i, sizeof(int)) < 0) {
    printf("Error leyendo cantidad de líneas del worker %d\n", pipeNumber);
    exit(1);
  }
  readLines[pipeNumber] = i;
}

void addEnergiesToMaterial(material* mat, double* energies) {
  for (int i = 0; i < mat->cellCount; i++) {
    mat->cells[i] = mat->cells[i] + energies[i];
  }
}

void addCompositeEnergiesToMaterial(material* mat, double* energies, int index, int subsetSize) {
  int cap = subsetSize;
  if (mat->cellCount - index < subsetSize) {
    cap = mat->cellCount - index;
  }

  for (int i = 0; i < cap; i++) {
    mat->cells[index + i] += energies[i];
  }

}

void readEnergies(material* mat, int** processPipes, int pipeNumber) {  
  // Caso en el que el total de celdas es suficiente para enviarse
  // Por el pipe
  if (mat->cellCount < 5000) {
    double energies[mat->cellCount];
    //double* energies = (double*)malloc(sizeof(double) * mat->cellCount);
    if (read(processPipes[PIPE_TO_P][PIPE_READ], energies, sizeof(double) * mat->cellCount) < 0) {
      printf("Error leyendo cantidad de líneas del worker %d\n", pipeNumber);
      exit(1);
    }
    addCompositeEnergiesToMaterial(mat, energies, 0, mat->cellCount);
    return;
  }
  // Caso en el que existen más celdas que las soportadas por
  // el pipe
  for (int i = 0; i < mat->cellCount; i += SUBSET_SIZE) {
    double energies[SUBSET_SIZE];
    //double* energies = (double*)malloc(sizeof(double) * SUBSET_SIZE);
    if (read(processPipes[PIPE_TO_P][PIPE_READ], energies, sizeof(double) * SUBSET_SIZE) < 0) {
      printf("Error leyendo cantidad de líneas del worker %d\n", pipeNumber);
      exit(1);
    }
    addCompositeEnergiesToMaterial(mat, energies, i, SUBSET_SIZE);
  }
  
  
}

void finishWorkers(material* mat, int workerCount, int*** pipes, int* readLines) {
  for (int i = 0; i < workerCount; i++) {
    write(pipes[i][PIPE_TO_C][PIPE_WRITE], "FIN", BUFFER_MAX * sizeof(char));
    // Lectura de número de líneas leídas
    readReadLines(readLines, pipes[i], i);
    readEnergies(mat, pipes[i], i);
  }
}

void printWorkers(int workerCount, int* pids, int* readLines, int fdMain) {
  char line[128];
  for (int i = 0; i < workerCount; i++) {
    sprintf(line, "El worker %d con pid %d leyo %d lineas\n", i + 1, pids[i], readLines[i]);
    write(fdMain, line, strlen(line));
  }
  return;
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

void printCell(int cellPos, int energyChars, int cellsChars, double cellEnergy, int cols, int fdMain) {
  char line[cols];
  sprintf(line, "%i", cellPos);
  write(fdMain, line, strlen(line));
  int cellChars = getCharAmount(cellPos);
  int cellEnergyChars = getCharAmount(cellEnergy);
  for (int i = 0; i < cellsChars - cellChars + 1; i++) {
    write(fdMain, " ", strlen(" "));
  }
  for (int i = 0; i < energyChars - cellEnergyChars; i++) {
    write(fdMain, " ", strlen(" "));
  }
  sprintf(line, "%.4f |", cellEnergy);
  write(fdMain, line, strlen(line));
  return;
}

void printCellEnergy(double cellEnergy, double part, int fdMain) {
  while (cellEnergy > part) {
    cellEnergy = cellEnergy - part;
    write(fdMain, "o", strlen("o"));
  }
  write(fdMain, "\n", strlen("\n"));
  return;
}

void printEnergy(material* mat, int fdMain) {
  struct winsize w;
  double maxEnergy = mat->cells[getMaxEnergyCellPos(mat)];
  int energyChars = getCharAmount(maxEnergy); 
  int cellsChars = getCharAmount(mat->cellCount);
  ioctl(0, TIOCGWINSZ, &w); // Obtiene información de la consola
  // Define cuánta energía equivale a cada o mostrado por consola
  double part = maxEnergy / (w.ws_col - (8 + energyChars + cellsChars));

  for (int i = 0; i < mat->cellCount; i++) {
    printCell(i, energyChars, cellsChars, mat->cells[i], w.ws_col, fdMain);
    printCellEnergy(mat->cells[i], part, fdMain);
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
  return;
}

void freeMaterial(material* mat) {
  free(mat->cells);
  free(mat);
}