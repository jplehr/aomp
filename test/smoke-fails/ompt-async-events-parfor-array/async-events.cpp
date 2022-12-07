#include <cstdlib>
#include <iostream>

#include "callbacks.h"

extern "C" void consume(int *p);

int identity(int i) { return i; }

/// @brief This version maps one larger array and works on each entry as opposed
/// to individual integers
/// @param argc
/// @param argv
/// @return
int main(int argc, char **argv) {
  const int threadParallel = 4;
  const int deviceStress = 1;

// We should see 3 * threadParallel * deviceStress tracing events
#pragma omp parallel for schedule(static,1)
  for (int i = 0; i < threadParallel; ++i) {
    int vals[deviceStress] = {0};
    int vals2[deviceStress] = {0};
    int vals3[deviceStress] = {0};

#pragma omp target data map(vals,vals2)
{
#pragma omp target teams distribute nowait
    for (int j = 0; j < deviceStress; ++j) {
      vals[j] += 1;
    }
#pragma omp target teams distribute nowait
    for (int j = 0; j < deviceStress; ++j) {
      vals2[j] += 1;
    }
}

#pragma omp target teams distribute map(vals3)
    for(int j = 0; j< deviceStress; ++j) {
      vals3[j] += 2;
    }

    for (int j = 0; j < deviceStress; ++j) {
      if (vals3[j] != 2) {
        std::cout << "MISMATCH " << vals3[j] << std::endl;
        abort();
      }
    }
  }

  std::cout << "Success" << std::endl;

  return 0;
}
