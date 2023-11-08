#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>
#include <string.h>

// Entradas: int, int, int, int
// Salidas: double
// Descripción: hace el calculo de la formula de impacto con los datos ingresados
double impactFormula(int cellPos, int impactedCellPos, int cellEnergy, int cellCount);
// Entradas: double*, int, int, int
// Salidas: void
// Descripción: aplica la formula de impacto en un arreglo de energía
void impactMaterial(double* energies, int cellCount, int particlePos, int particleEnergy);
// Entradas: int
// Salidas: double*
// Descripción: crea un arreglo de energía, inicializando todos los valores en 0
double* createEnergyArray(int cellCount);
// Entradas: double*, double*, int, int, int
// Salidas: void
// Descripción: Asigna la energía correspondiente a un subconjunto
void subsetEnergies(double* energies, double* subset, int cellCount, int index, int subsetSize);
// Entradas: int, double*, int, int
// Salidas: void
// Descripción: Envía la energía calculada por el worker, en paquetes de menor tamaño
void sendEnergiesToPipe(int fdToParent, double* energies, int cellCount, int subsetSize);
