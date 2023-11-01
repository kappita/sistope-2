#include "fworker.h"


double impactFormula(int cellPos, int impactedCellPos, int cellEnergy, int cellCount) {
  double result = (pow(10, 3) * cellEnergy) /
                  (cellCount * pow(abs(impactedCellPos - cellPos) + 1, 0.5));
  if (result < (pow(10, -3) / cellCount)) {
    return (double)0;
  };
  return result;
}


void impactMaterial(double* energies, int cellCount, int particlePos, int particleEnergy) {
  // Se aplica la fórmula de impacto para cada celda del material
  for (int i = 0; i < cellCount; i++) {
    double impact = impactFormula(i, particlePos, particleEnergy, cellCount);
    energies[i] = energies[i] + impact;
  }
  return;
}


double* createEnergyArray(int size) {
  double* energies = (double*)malloc(sizeof(double) * size);
  for (int i = 0; i < size; i++) {
    energies[i] = 0;
  }
  return energies;
}


void subsetEnergies(double* energies, double* subset, int cellCount, int index, int subsetSize) {
  int cap = subsetSize;
  if (cellCount - index < subsetSize) {
    cap = cellCount - index;
  }

  for (int i = 0; i < cap; i++) {
    subset[i] = energies[index + i];
  }
}

void sendEnergiesToPipe(int fdToParent, double* energies, int cellCount, int subsetSize) {
  double* energySubset = createEnergyArray(subsetSize);
  for (int i = 0; i < cellCount; i += subsetSize) {
    subsetEnergies(energies, energySubset, cellCount, i, subsetSize);
    write(fdToParent, energySubset, sizeof(double) * subsetSize);
  }
  free(energySubset);
}

