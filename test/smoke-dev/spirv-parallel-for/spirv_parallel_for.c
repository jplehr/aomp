// Simple target parallel for test for SPIR-V offloading.
// Uses return code for validation instead of printf (no stdio.h).

#define N 1024

int main() {
  int a[N];
  int b[N];
  int errors = 0;

  for (int i = 0; i < N; i++)
    a[i] = i;

  #pragma omp target parallel for map(to: a[:N]) map(from: b[:N])
  for (int i = 0; i < N; i++)
    b[i] = a[i] * 2;

  for (int i = 0; i < N; i++) {
    if (b[i] != i * 2)
      errors++;
  }

  return errors;
}
