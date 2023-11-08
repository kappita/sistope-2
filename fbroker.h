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


// Entrada: int
// Salida: material*
// Descripción: se le asigna memoria a las celdas a una variable tipo
// material que contiene el numero de celdas y el arreglo con dicha cantidad de celdas
material* createMaterial(int cellCount);

// Entrada: vacio
// Salida: un nuevo pipe creado
// Descripción: se asigna memoria a un nuevo pipe si este no fue creado
// exitosamente entonces muestra un mensaje de error y termina la funcion
// caso contrario retorna el nuevo pipe creado
int* createSinglePipe();
// Entrada: vacio
// Salida: retorna un arreglo de entero pipes
// Descripción: Se crea un par de pipes, para la comunicación
// bidireccional entre procesos
int** createDualPipe();
// Entrada: int
// Salida: int***
// Descripción: se le asigna memoria a un puntero triple con la cantidad
// de pipes a hacer, en cada celda tiene doble pipes y si no hay errores
// lo retorna 
int*** createPipes(int workerCount);
// Entrada: int***, int
// Salida: void
// Descripción: Libera la memoria alojada por los pipes
void freePipes(int*** pipes, int workerCount);
// Entrada: int, char*, char*, int***, int*
// Salida: void 
// Descripción: se crean los workers y se les entregan los argumentos
// necesarios
void createWorkers(int workerCount, char* cellCount, char* chunk, int*** pipes, int* pids);
// Entrada: int**, char*
// Salida: void 
// Descripción: Envía una línea de texto a un worker
void sendLineToWorker(int** processPipes, char* line);
// Entrada: int*, int**, int
// Salida: void 
// Descripción: lee la cantidad de lineas procesadas por un hijo
void readReadLines(int* readLines, int** processPipes, int pipeNumber);

// Entrada: material*, double*
// Salida: void
// Descripción: agrega a cada una de las celdas del material las energias correspondientes
void addEnergiesToMaterial(material* mat, double* energies);
// Entrada: material*, double*, int, int
// Salida: void
// Descripción: agrega un subarreglo de energía al material
void addCompositeEnergiesToMaterial(material* mat, double* energies, int index, int subsetSize);
// Entrada: material*, int**, int
// Salida: void 
// Descripción: lee el arreglo de energías del proceso hijo y los agrega al material
void readEnergies(material* mat, int** processPipes, int pipeNumber);
// Entrada: material*, int, int***, int*
// Salida: void
// Descripción: Manda una señal a los workers para que terminen de 
// leer líneas y lee sus resultados.
void finishWorkers(material* mat, int workerCount, int*** pipes, int* readLines);
// Entrada: int, int*, int*, int
// Salida: void
// Descripción: muestra cuantas lineas leyo el pid de cada procesador o trabajador 
void printWorkers(int workerCount, int* pids, int* readLines, int fdMain);
// Entradas: Material*
// Salidas: int
// Descripción: Obtiene la posición de la celda del material con mayor energía acumulada
int getMaxEnergyCellPos(material* mat);
// Entrada: double
// Salida: int
// Descripción: Retorna la cantidad de caracteres de un número
int getCharAmount(double number);

// Entrada: int, int, int, double, int, int
// Salida: void
// Descripción: Imprime la energía de una celda, de forma normalizada
void printCell(int cellPos, int energyChars, int cellsChars, double cellEnergy, int cols, int fdMain);
// Entrada: double, double, int
// Salida: void
// Descripción: Imprime de forma gráfica la energía de una celda
void printCellEnergy(double cellEnergy, double part, int fdMain);
// Entrada: material*, int
// Salida: void
// Descripción: imprime la energía almacenada por un material
void printEnergy(material* mat, int fdMain);
// Entrada: FILE*, int, int
// Salida: void
// Descripción: imprime la energia de una celda en el archivo de salida
void writeCell(FILE *fp, int cellPos, int energyChars, int maxCellsChars, double cellEnergy);
// Entrada: FILE*, material*
// Salida: void
// Descripción: imprime la energía almacenada en un material en el archivo de salida
void writeEnergy(FILE *fp, material* mat);
// Entrada: material*
// Salida: void
// Descripción: libera el espacio de memoria ocupado por material
void freeMaterial(material* mat);