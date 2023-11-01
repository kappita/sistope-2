#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>
#include <string.h>
#include <sys/ioctl.h>
#include "structs.h"

#define BUFFER_MAX 64
#define PIPE_TO_C 0
#define PIPE_TO_P 1

#define PIPE_READ 0
#define PIPE_WRITE 1


material* createMaterial(int cellCount);
int*** createPipes(int workerCount);
void createWorkers(int workerCount, char* cellCount, char* chunk, int*** pipes, int* pids);
void finishWorkers(material* mat, int workerCount, int*** pipes, int* readLines);
void sendLineToWorker(int** processPipes, char* line);
void readReadLines(int* readLines, int** processPipes, int pipeNumber);
void readEnergies(material* mat, int** processPipes, int pipeNumber);
void addEnergiesToMaterial(material* mat, double* energies);
void printWorkers(int workerCount, int* pids, int* readLines, int fdMain);
void writeCell(FILE *fp, int cellPos, int energyChars, int maxCellsChars, double cellEnergy);
void writeEnergy(FILE *fp, material* mat);
void printCell(int cellPos, int energyChars, int cellsChars, double cellEnergy, int cols, int fdMain);
void printCellEnergy(double cellEnergy, double part, int fdMain);
void printEnergy(material* mat, int fdMain);
void freeMaterial(material* mat);