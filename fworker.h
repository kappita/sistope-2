#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>
#include <string.h>

double impactFormula(int cellPos, int impactedCellPos, int cellEnergy, int cellCount);
void impactMaterial(double* energies, int cellCount, int particlePos, int particleEnergy);
double* createEnergyArray(int cellCount);
void sendEnergiesToPipe(int fdToParent, double* energies, int cellCount, int subsetSize);
