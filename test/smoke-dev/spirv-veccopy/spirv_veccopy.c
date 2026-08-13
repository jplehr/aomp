// Simple vector copy test for SPIR-V offloading.
// Uses return code for validation instead of printf (no stdio.h).

#define N 1024

int main() {
  int a[N];
  int b[N];
  int errors = 0;

  for (int i = 0; i < N; i++)
    a[i] = i;

  #pragma omp target teams distribute parallel for map(to: a[:N]) map(from: b[:N])
  for (int i = 0; i < N; i++)
    b[i] = a[i];

  for (int i = 0; i < N; i++) {
    if (b[i] != i)
      errors++;
  }

  return errors;
}
