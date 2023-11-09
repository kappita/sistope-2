#include "fbroker.h"

// Entradas: Cantidad de celdas, cantidad de workers,
//           ruta del archivo de entrada, ruta del
//           archivo de salida, cantidad de chunks,
//           bool de mensaje en consola, descriptor
//           del proceso main 
// Salida: int
// Descripción: Ejecuta el broker
int main(int argc, char* argv[]) {
  // Lectura de argumentos
  int cellCount = atoi(argv[1]);
  int workerCount = atoi(argv[2]);
  char* inPath = argv[3];
  char* outPath = argv[4];
  int chunk = atoi(argv[5]);
  int consoleMessage = atoi(argv[6]);
  int fdMain = atoi(argv[7]);

  // Buffers
  char fdCBuffer[BUFFER_MAX];

  // Información de los workers
  int*** pipes = createPipes(workerCount);
  int* pids = (int*)malloc(sizeof(int)* workerCount);
  int* readLines = (int*)malloc(sizeof(int) * workerCount);

  createWorkers(workerCount, argv[1], argv[5], pipes, pids);

  // Apertura de archivos
  FILE* in = fopen(inPath, "r");
  FILE* out = fopen(outPath, "w");

  // Material
  material* mat = createMaterial(cellCount);

  int linesRead;
  int turn;
  linesRead = 0;
  turn = rand() % workerCount;
  // Se ignora la cantidad de líneas del archivo
  fgets(fdCBuffer, BUFFER_MAX, in);
  // Se lee hasta que no queden líneas sin leer
  while (fgets(fdCBuffer, BUFFER_MAX, in) != NULL) {
    linesRead++;
    sendLineToWorker(pipes[turn], fdCBuffer);
    // Si se alcanza el tamaño del chunk, se reasigna el turno a otro worker
    if (linesRead == chunk) {
      linesRead = 0;
      turn = rand() % workerCount;
    }
  }
  fclose(in);
  // Se leen los resultados de los workers
  finishWorkers(mat, workerCount, pipes, readLines);
  writeEnergy(out, mat);
  fclose(out);

  if (consoleMessage) {
    printWorkers(workerCount, pids, readLines, fdMain);
    printEnergy(mat, fdMain);
  }
  
  freeMaterial(mat);
  freePipes(pipes, workerCount);
  
  return 0;
}

