#include <cstdlib>

#include "callbacks.h"

extern "C" void consume(int *p);

int identity(int i) { return i; }

/// @brief This version maps one larger array and works on each entry as opposed
/// to individual integers
/// @param argc
/// @param argv
/// @return
int main(int argc, char **argv) {
  const int threadParallel = 256;
  const int deviceStress = 2;

// We should see 3 * threadParallel * deviceStress tracing events
#pragma omp parallel for schedule(static,1)
  for (int i = 0; i < threadParallel; ++i) {
    int vals[deviceStress] = {0};

#pragma omp target teams distribute map(vals)
    for (int j = 0; j < deviceStress; ++j) {
      vals[j] = 1;
    }

    for (int j = 0; j < deviceStress; ++j) {
      consume(&vals[j]);

      if (vals[j] != 1) {
        abort();
      }
    }
  }

  return 0;
}
