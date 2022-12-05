#include <cstdlib>

extern "C" void consume(int *p);

int main(int argc, char **argv) {
  const int N = 1024;

#pragma omp parallel
  {
#pragma omp single
    {
      for (int i = 0; i < N; ++i) {
#pragma omp task
        {
          int l_val = i;
#pragma omp target map(l_val)
          { l_val += 1; }
          if (l_val != i+1) {
            abort();
          }
          consume (&l_val);
        }
      }
    }
  }
  return 0;
}