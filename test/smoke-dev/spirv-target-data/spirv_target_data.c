// Test target data region with explicit data mapping for SPIR-V.
// Uses return code for validation instead of printf (no stdio.h).

#define N 256

int main() {
  int a[N];
  int b[N];
  int c[N];
  int errors = 0;

  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = N - i;
  }

  #pragma omp target data map(to: a[:N], b[:N]) map(from: c[:N])
  {
    #pragma omp target parallel for
    for (int i = 0; i < N; i++)
      c[i] = a[i] + b[i];
  }

  for (int i = 0; i < N; i++) {
    if (c[i] != N)
      errors++;
  }

  return errors;
}
