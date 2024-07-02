#include <cstring>
#include <stdio.h>
#include "omp.h"

#include "OmptTester.h"

using namespace omptest;
using namespace internal;

// Used in Test case later in the file
#define X 100000
#pragma omp declare target
int c[X];
#pragma omp end declare target


TEST(VeccopyCallbacks, OnHost_one) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);

  int N = 100000;
  int i;
  int numThreads=2;

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_ASSERT_SET(ThreadBegin, ompt_thread_worker);
#pragma omp parallel for num_threads(numThreads)
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }
}

TEST(VeccopyCallbacks, OnDevice_Sequenced) {

  int N = 100000;
  int i;

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SEQUENCE(Target, TARGET, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int)); // a ?
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), &a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int)); // b ?
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), &b);
  OMPT_ASSERT_SEQUENCE(TargetSubmit, 1);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N * sizeof(int), nullptr, &b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N* sizeof(int), nullptr, &a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0); // Why 0?
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0); // Why 0?
  OMPT_ASSERT_SEQUENCE(Target, TARGET, END, 0);

#pragma omp target parallel for
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }

  OMPT_ASSERT_SEQUENCE(Target, TARGET, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int)); // a ?
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), &a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int)); // b ?
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), &b);
  OMPT_ASSERT_SEQUENCE(TargetSubmit, 3);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N * sizeof(int), nullptr, &b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N* sizeof(int), nullptr, &a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0); // Why 0?
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0); // Why 0?
  OMPT_ASSERT_SEQUENCE(Target, TARGET, END, 0);

#pragma omp target teams distribute parallel for num_teams(3)
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }

  int rc = 0;
  for (i=0; i<N; i++) ;
    // EXPECT_EQ(a[i], b[i]);
}

TEST(VeccopyCallbacks, OnDevice_SequencedGrouped) {
  /* The Test Body */

  // Define testcase variables before events, so we can refer to them.
  int N = 100000;
  int a[N];
  int b[N];

  /* First Target Region */

  OMPT_ASSERT_SEQUENCE_GROUPED("R1", Target, /*Kind=*/TARGET,
                               /*Endpoint=*/BEGIN, /*DeviceNum=*/0)

  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/ALLOC,
                               /*Size=*/N * sizeof(int))
  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/H2D,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/&a)

  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/ALLOC,
                               /*Size=*/N * sizeof(int))
  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/H2D,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/&b)

  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetSubmit, /*RequestedNumTeams=*/1)

  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/D2H,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/nullptr, /*DstAddr=*/&b)
  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/D2H,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/nullptr, /*DstAddr=*/&a)

  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/DELETE,
                               /*Size=*/0)
  OMPT_ASSERT_SEQUENCE_GROUPED("R1", TargetDataOp, /*OpType=*/DELETE,
                               /*Size=*/0)

  OMPT_ASSERT_SEQUENCE_GROUPED("R1", Target, /*Kind=*/TARGET, /*Endpoint=*/END,
                               /*DeviceNum=*/0)

  /* Second Target Region */

  OMPT_ASSERT_SEQUENCE_GROUPED("R2", Target, /*Kind=*/TARGET,
                               /*Endpoint=*/BEGIN, /*DeviceNum=*/0)

  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/ALLOC,
                               /*Size=*/N * sizeof(int))
  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/H2D,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/&a)

  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/ALLOC,
                               /*Size=*/N * sizeof(int))
  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/H2D,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/&b)

  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetSubmit, /*RequestedNumTeams=*/0)

  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/D2H,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/nullptr, /*DstAddr=*/&b)
  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/D2H,
                               /*Size=*/N * sizeof(int),
                               /*SrcAddr=*/nullptr, /*DstAddr=*/&a)

  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/DELETE,
                               /*Size=*/0)
  OMPT_ASSERT_SEQUENCE_GROUPED("R2", TargetDataOp, /*OpType=*/DELETE,
                               /*Size=*/0)

  OMPT_ASSERT_SEQUENCE_GROUPED("R2", Target, /*Kind=*/TARGET, /*Endpoint=*/END,
                               /*DeviceNum=*/0)

  /* Actual testcase code. */

  for (int i = 0; i < N; i++)
    a[i] = 0;

  for (int i = 0; i < N; i++)
    b[i] = i;

#pragma omp target parallel for
  {
    for (int j = 0; j < N; j++)
      a[j] = b[j];
  }

#pragma omp target teams distribute parallel for
  {
    for (int j = 0; j < N; j++)
      a[j] = b[j];
  }

  int rc = 0;
  for (int i = 0; i < N; i++)
    if (a[i] != b[i]) {
      rc++;
      printf("Wrong value: a[%d]=%d\n", i, a[i]);
    }

  if (!rc)
    printf("Success\n");
}

TEST(VeccopyCallbacks, ExplicitDefaultDevice) {
  int N = 100000;

  int a[N];
  int b[N];

  int i;

  for (i = 0; i < N; i++)
    a[i] = 0;

  for (i = 0; i < N; i++)
    b[i] = i;

  int dev=  omp_get_default_device();

  OMPT_ASSERT_SEQUENCE(Target, TARGET, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int), a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int), b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), b);
  OMPT_ASSERT_SEQUENCE(TargetSubmit, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N * sizeof(int), nullptr, b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N * sizeof(int), nullptr, a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  OMPT_ASSERT_SEQUENCE(Target, TARGET, END, 0);

    #pragma omp target teams distribute parallel for device(dev)
    {
      for (int j = 0; j < N; j++)
        a[j] = b[j];
    }

  for (i = 0; i < N; i++)
    ;// EXPECT_EQ(a[i] == b[i]);
}

TEST(VeccopyCallbacks, MultipleDevices) {
  int N = 100000;

  int a[N];
  int b[N];

  int i;

  for (i = 0; i < N; i++)
    a[i] = 0;

  for (i = 0; i < N; i++)
    b[i] = i;

for (int dev = 0; dev < omp_get_num_devices(); ++dev) {
  OMPT_ASSERT_SEQUENCE(Target, TARGET, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int), a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, N * sizeof(int), b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, N * sizeof(int), b);
  OMPT_ASSERT_SEQUENCE(TargetSubmit, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N * sizeof(int), nullptr, b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, N * sizeof(int), nullptr, a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  OMPT_ASSERT_SEQUENCE(Target, TARGET, END, 0);

    #pragma omp target teams distribute parallel for device(dev)
    {
      for (int j = 0; j < N; j++)
        a[j] = b[j];
    }

    OMPT_ASSERT_SYNC_POINT("After target loop for device " + std::to_string(dev));

  for (i = 0; i < N; i++)
    ;// EXPECT_EQ(a[i] == b[i]);
  } // device loop
}

TEST(DataOpTests, DeclareTargetGlobalAndUpdate) {
  int a[X];
  int b[X];

  int i;

  for (i = 0; i < X; i++)
    a[i] = 0;

  for (i = 0; i < X; i++)
    b[i] = i;

  for (i = 0; i < X; i++)
    c[i] = 0;

  OMPT_ASSERT_SEQUENCE(Target, ENTER_DATA, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, X * sizeof(int), a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, X * sizeof(int), a);
  OMPT_ASSERT_SEQUENCE(Target, ENTER_DATA, END, 0);

#pragma omp target enter data map(to : a)
  OMPT_ASSERT_SYNC_POINT("After enter data");

  OMPT_ASSERT_SEQUENCE(Target, TARGET, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, X * sizeof(int), b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, X * sizeof(int), b);
  OMPT_ASSERT_SEQUENCE(TargetSubmit, 1);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, X * sizeof(int), nullptr, b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  OMPT_ASSERT_SEQUENCE(Target, TARGET, END, 0);
#pragma omp target parallel for
  {
    for (int j = 0; j < X; j++)
      a[j] = b[j];
  }
  OMPT_ASSERT_SYNC_POINT("After target parallel for");

  OMPT_ASSERT_SEQUENCE(Target, EXIT_DATA, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, X * sizeof(int), nullptr, a);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  OMPT_ASSERT_SEQUENCE(Target, EXIT_DATA, END, 0);
#pragma omp target exit data map(from : a)
  OMPT_ASSERT_SYNC_POINT("After target exit data");

  OMPT_ASSERT_SEQUENCE(Target, TARGET, BEGIN, 0);
  // FIXME: This is currently not occuring. Is this a bug?
  // OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, X * sizeof(c));
  OMPT_ASSERT_SEQUENCE(TargetSubmit, 1);
  OMPT_ASSERT_SEQUENCE(Target, TARGET, END, 0);
#pragma omp target parallel for map(alloc : c)
  {
    for (int j = 0; j < X; j++)
      c[j] = 2 * j + 1;
  }
  OMPT_ASSERT_SYNC_POINT("After target parallel for");

  OMPT_ASSERT_SEQUENCE(Target, UPDATE, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, X * sizeof(int), nullptr, c);
  OMPT_ASSERT_SEQUENCE(Target, UPDATE, END, 0);
#pragma omp target update from(c) nowait
#pragma omp barrier

  for (i = 0; i < X; i++){ ;
//    EXPECT_EQ (a[i] == i);
//    EXPECT_EQ (c[i] == 2 * i + 1)
  }
}

TEST(DataOpTests, ExplicitAllocatorAPI) {
  int * d_a = nullptr;
  const int N = 1000;
  auto DataSize = N * sizeof(int);

  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, DataSize);
  d_a = (int *) omp_target_alloc(DataSize, omp_get_default_device());
  assert(d_a != nullptr);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  omp_target_free(d_a, omp_get_default_device());
}

TEST(DataOpTests, ExplicitAllocatorAndCopyAPI) {
  int *d_a = nullptr;
  int *d_b = nullptr;
  const int N = 1000;
  auto DataSize = N * sizeof(int);

  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, DataSize, d_b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, DataSize, d_b);
  d_a = (int *) omp_target_alloc(DataSize, omp_get_default_device());
  d_b = (int *) omp_target_alloc(DataSize, omp_get_default_device());

  OMPT_ASSERT_SEQUENCE(TargetDataOp, D2H, DataSize, d_a, d_b);
  omp_target_memcpy(d_b, d_a , DataSize , 0, 0, omp_get_default_device(), omp_get_default_device());

  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0, d_b);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0, d_a);
  omp_target_free(d_b, omp_get_default_device());
  omp_target_free(d_a, omp_get_default_device());
}

TEST(DataOpTests, ExplicitAllocatorAndUpdate) {
  int *p_a = nullptr;
  int *d_a = nullptr;
  const int N = 1000;
  auto DataSize = N * sizeof(int);
  p_a = (int *) malloc(DataSize);
  memset(p_a, 0, N * sizeof(int));

  OMPT_ASSERT_SEQUENCE(TargetDataOp, ALLOC, DataSize, d_a);
  d_a = (int *) omp_target_alloc(DataSize, omp_get_default_device());
  OMPT_ASSERT_SEQUENCE(TargetDataOp, ASSOCIATE, DataSize, d_a);
  omp_target_associate_ptr(p_a , d_a , DataSize, 0 , omp_get_default_device());

  OMPT_ASSERT_SEQUENCE(Target, UPDATE, BEGIN, 0);
  OMPT_ASSERT_SEQUENCE(TargetDataOp, H2D, DataSize, p_a);
  OMPT_ASSERT_SEQUENCE(Target, UPDATE, END, 0);
  #pragma omp target update to(d_a)

  OMPT_ASSERT_SEQUENCE(TargetDataOp, DISASSOCIATE, DataSize, d_a);
  omp_target_disassociate_ptr(d_a, omp_get_default_device());
  OMPT_ASSERT_SEQUENCE(TargetDataOp, DELETE, 0);
  omp_target_free(d_a, omp_get_default_device());
  free(p_a);
}

// FIXME: Leave this suite here, so it gets discovered last and executed first.
TEST(InitFiniSuite, DeviceLoad) {
  /* The Test Body */
  OMPT_SUPPRESS_EVENT(EventTy::Target)
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp)
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit)

  int N = 128;
  int a[N];

  OMPT_ASSERT_SEQUENCE(DeviceLoad, /*DeviceNum=*/0)

#pragma omp target parallel for
  {
    for (int j = 0; j < N; j++)
      a[j] = 0;
  }
}

int main(int argc, char **argv) {
  Runner R;
  R.run();

  return 0;
}
