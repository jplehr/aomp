#include <cstdlib>

#include "callbacks.h"

extern "C" void consume(int *p);

int main(int argc, char **argv) {
  const int threadParallel = 2;
  const int deviceStress = 4;

/// One thread submits many asynchronous tasks
#pragma omp parallel
  {
#pragma omp single
    {
      for (int i = 0; i < threadParallel; ++i) {
#pragma omp task
        {
          for (int j = 0; j < deviceStress; ++j) {
            int l_val = i;
#pragma omp target map(l_val)
            { l_val += 1; }
            if (l_val != i + 1) {
              abort();
            }
            consume(&l_val);
          }
        }
      }
    }
  }
  return 0;
}
