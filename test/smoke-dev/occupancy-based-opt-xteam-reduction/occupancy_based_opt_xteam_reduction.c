#include <stdio.h>
#include <omp.h>

int main()
{
  int N = 1000000;

  double a[N];

  for (int i=0; i<N; i++)
    a[i]=i;

  double sum1, sum2, sum3, sum4;
  sum1 = sum2 = sum3 = sum4 = 0;
  
#pragma omp target teams distribute parallel for map(tofrom:sum1) reduction(+:sum1)
  for (int j = 0; j< N; j=j+1)
    sum1 += a[j];

#pragma omp target teams distribute parallel for map(tofrom:sum2) reduction(+:sum2)
  for (int j = 0; j< N; j=j+2)
    sum2 += a[j];

#pragma omp target teams distribute parallel for map(tofrom:sum3,sum4) reduction(+:sum3,sum4)
  for (int j = 0; j< N; j=j+1) {
    sum3 += a[j];
    sum4 += a[j];
  }
  
  printf("%f %f %f %f\n", sum1, sum2, sum3, sum4);
  
  int rc =
    (sum1 != 499999500000) ||
    (sum2 != 249999500000) ||
    (sum3 != 499999500000) ||
    (sum4 != 499999500000);

  if (!rc)
    printf("Success\n");

  return rc;
}

/// CHECK: Achieved Occupancy: 100%
/// CHECK: Achieved Occupancy: 100%
/// CHECK: Achieved Occupancy: 100%

