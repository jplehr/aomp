#include <cstdlib>

extern "C" void consume(int *p);

int identity(int i) { return i; }

int main(int argc, char **argv) {
  const int threadParallel = 128;
  const int deviceStress = 1000;

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

#pragma omp parallel for
  for (int i = 0; i < threadParallel; ++i) {
    for (int j = 0; j < deviceStress; ++j) {
      int l_t = identity(i);
#pragma omp target map(l_t)
      { l_t += 1; }
      consume(&l_t);
      if (l_t != i + 1) {
        abort();
      }
    }
  }
  return 0;
}
