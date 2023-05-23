//From AOMP issue #198
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct S {
  int *arr;
};

#pragma omp declare target
S ps;
#pragma omp end declare target

int main(void)
{
  // Allocate array and set to zero
  int len = 6;
  ps.arr = (int *) calloc( len, sizeof(int) );

  // Map and fill with values on device
  #pragma omp target teams distribute parallel for map(tofrom: ps.arr[:len])
  for( int i = 0; i < len; i++)
  {
    ps.arr[i] = i;
  }

  assert(ps.arr[5] == 5 && "Results are Incorrect");

  return 0;
}
