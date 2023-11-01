#include <getopt.h>
#include "functions.h"
#include <unistd.h>
#include <wait.h>

#define BUFFERMAX 16

int main(int argc, char* argv[]) {
  int option;
  char* cells;
  char* workers;
  char* chunks;
  int gotIn = 0, gotOut = 0, gotCells = 0, gotWorkers = 0, gotChunks = 0;
  char *inPath, *outPath;
  int console = 0;


  // Lee el getopt y verifica si cada opción es válida
  while((option = getopt(argc, argv, "N:P:i:o:c:D")) != -1) {
    switch (option) {
      case 'N':
        if (!handleNumber(optarg)) {
          return 1;
        }
        cells = optarg;
        gotCells = 1;
        break;

      case 'P':
        if (!handleNumber(optarg)) {
          return 1;
        }
        workers = optarg;
        gotWorkers = 1;
        break;

      case 'i':
        if (!handleString(optarg)) {
          return 1;
        }
        gotIn = 1;
        inPath = optarg;
        break;

      case 'o':
        if (!handleString(optarg)) {
          return 1;
        };
        gotOut = 1;
        outPath = optarg;
        break;

      case 'c':
        if (!handleNumber(optarg)) {
          return 1;
        }
        chunks = optarg;
        gotChunks = 1;
        break;

      case 'D':
        console = 1;
        break;
    } 
  }



  // En caso de que no se haya ingresado información necesaria
  if (gotCells == 0 || gotIn == 0 || gotWorkers == 0 || gotChunks == 0) {
    printf("Argumentos insuficientes\n");
    return 1;
  }

  if (gotOut == 0) {
    outPath = "nashe.txt";
  }


  int pid = fork();
  int fd[2];
  if (pipe(fd) == -1) {
    printf("Error al crear el pipe\n");
    exit(1);
  }


  dup2(STDOUT_FILENO, fd[1]);


  if (pid < 0) {
    printf("Error creando el proceso hijo\n");
    return 1;
  } else if (pid == 0) {
    // Lógica del proceso hijo
    char strConsole[BUFFERMAX];
    char strFdMain[BUFFERMAX];
    sprintf(strConsole, "%d", console);
    sprintf(strFdMain, "%d", fd[1]);
    printf("%s", cells);
    char* args[] = {"broker", cells, workers, inPath, outPath, chunks, strConsole, strFdMain, NULL};
    int bruh = execv("./broker", args);
    
  } else {
    // Lógica del proceso padre.

    wait(NULL);
  }

  return 0;
}