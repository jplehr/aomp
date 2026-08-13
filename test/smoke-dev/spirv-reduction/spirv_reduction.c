// Test reduction clause for SPIR-V offloading.
// Uses return code for validation instead of printf (no stdio.h).

#define N 1024

int main() {
  int a[N];
  int sum = 0;
  int expected = 0;

  for (int i = 0; i < N; i++) {
    a[i] = i;
    expected += i;
  }

  #pragma omp target parallel for reduction(+:sum) map(to: a[:N])
  for (int i = 0; i < N; i++)
    sum += a[i];

  if (sum != expected)
    return 1;

  return 0;
}
