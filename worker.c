#include "fworker.h"

#define BUFFER_MAX 64
#define PIPE_READ 0
#define PIPE_WRITE 1



int main(int argc, char* argv[]) {
  int cellCount = atoi(argv[1]);

  char fdCBuffer[BUFFER_MAX];
  int fd[2];
  // fd de lectura -> pipe[PIPE_TO_C][PIPE_READ]
  fd[PIPE_READ] = atoi(argv[2]);
  // fd de escritura -> pipe[PIPE_TO_P][PIPE_WRITE]
  fd[PIPE_WRITE] = atoi(argv[3]);
  int subsetSize = atoi(argv[4]);
  int done = 0;
  int readLines = 0;
  double* energies = createEnergyArray(cellCount);
  int impactPos;
  int impactEnergy;

  while (!done) {
    if (read(fd[PIPE_READ], fdCBuffer, BUFFER_MAX * sizeof(char)) < 0) {
      printf("Error leyendo la línea enviada por el broker\n");
      exit(1);
    }
    if (strcmp(fdCBuffer, "FIN") == 0) {
      printf("Finalizando worker\n");
      done = 1;
      continue;
    }

    if (sscanf(fdCBuffer, "%d %d", &impactPos, &impactEnergy) != 2) {
      printf("Error leyendo los números de la línea recibida\n");
      exit(1);
    }

    readLines++;
    impactMaterial(energies, cellCount, impactPos, impactEnergy);
  }
  // Se envían las líneas leídas por el worker
  write(fd[1], &readLines, sizeof(int));

  // Se segmenta el arreglo completo y se envía por partes
  sendEnergiesToPipe(fd[1], energies, cellCount, subsetSize);
  free(energies);
}