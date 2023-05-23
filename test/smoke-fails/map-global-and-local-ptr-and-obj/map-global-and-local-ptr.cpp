//From AOMP issue #198
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct TT {
  int *arr;
} s;

#pragma omp declare target
int * arr;
#pragma omp end declare target

int main(void)
{
  // Allocate array and set to zero
  int len = 6;
  arr = (int *) calloc( len, sizeof(int) );

  s.arr = (int *) calloc( 2*len, sizeof(int));

  // Map and fill with values on device
//#pragma omp target teams distribute parallel for map(tofrom: s.arr[:len])
//#pragma omp target teams distribute parallel for map(tofrom: arr[:len])
#pragma omp target teams distribute parallel for map(tofrom: arr[:len]) map(tofrom: s.arr[:2*len])
  for( int i = 0; i < len; i++)
  {
    arr[i] = i;
    s.arr[i] = 1;
  }

  assert(arr[5] == 5 && "Results are Incorrect");
  assert(s.arr[5] == 1 && "Results are Incorrect");

  return 0;
}
