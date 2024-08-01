#include "omp.h"

#include "OmptTester.h"

using namespace omptest;
using namespace internal;

// Used in Test case later in the file
#define X 100000
#pragma omp declare target
int c[X];
#pragma omp end declare target

TEST(DataOpCallbacks, DISABLED_ExplicitAllocatorAndUpdate_small) {
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);

  int *p_a = nullptr;
  int *d_a = nullptr;
  int DefaultDeviceNum = omp_get_default_device();
  const int N = 1000;
  auto DataSize = N * sizeof(int);
  p_a = (int *)malloc(DataSize);
  memset(p_a, 0, N * sizeof(int));

  d_a = (int *)omp_target_alloc(DataSize, DefaultDeviceNum);

  omp_target_associate_ptr(p_a, d_a, DataSize, 0, DefaultDeviceNum);

  // TODO is "no data operation" what one *should* expect?
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, H2D, BEGIN, DataSize, p_a);
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, H2D, END, DataSize, p_a);
#pragma omp target update to(d_a)

  omp_target_disassociate_ptr(p_a, DefaultDeviceNum);

  omp_target_free(d_a, DefaultDeviceNum);
  free(p_a);
}

TEST(DataOpCallbacks, DISABLED_ExplicitAllocatorAndUpdate) {
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  // OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);
  // OMPT_SUPPRESS_EVENT(EventTy::BufferRecordDealloc);
  // OMPT_PERMIT_EVENT(EventTy::BufferRecordDeallocation);

  int *p_a = nullptr;
  int *d_a = nullptr;
  int DefaultDeviceNum = omp_get_default_device();
  const int N = 1000;
  auto DataSize = N * sizeof(int);
  p_a = (int *)malloc(DataSize);
  memset(p_a, 0, N * sizeof(int));

  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ALLOC, BEGIN, DataSize);
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ALLOC, END, DataSize, d_a);

  d_a = (int *)omp_target_alloc(DataSize, DefaultDeviceNum);

  // #pragma omp target data map(alloc : d_a[0:N])

  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ASSOCIATE, BEGIN, DataSize, p_a, d_a);
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ASSOCIATE, END, DataSize, p_a, d_a);
  omp_target_associate_ptr(p_a, d_a, DataSize, 0, DefaultDeviceNum);

  // #pragma omp target update to(p_a[:4])
  // #pragma omp target update from(d_a[:2])

  // OMPT_ASSERT_SEQUENCE(TargetEmi, UPDATE, BEGIN, DefaultDeviceNum);

  // TODO is "no data operation" what one *should* expect?
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, H2D, BEGIN, DataSize, p_a);
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, H2D, END, DataSize, p_a);

  // OMPT_ASSERT_SEQUENCE(TargetEmi, UPDATE, END, DefaultDeviceNum);

// #pragma omp target enter data map(alloc : d_a)
// #pragma omp target data map(alloc : d_a[0:N])
#pragma omp target update to(p_a[0:N])
// #pragma omp target data exit map(alloc : X)

  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DISASSOCIATE, BEGIN, 0, p_a);
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DISASSOCIATE, END, 0, p_a);
  omp_target_disassociate_ptr(p_a, DefaultDeviceNum);

  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DELETE, BEGIN, 0, d_a);
  // OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DELETE, END, 0, d_a);
  omp_target_free(d_a, DefaultDeviceNum);
  free(p_a);
}

TEST(DataOpCallbacks, Target_Update_Ranged) {
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecordDeallocation);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);

  int *p_a = nullptr;
  int *d_a = nullptr;
  int DefaultDeviceNum = omp_get_default_device();
  const int N = 1000;
  auto DataSize = N * sizeof(int);
  p_a = (int *)malloc(DataSize);
  memset(p_a, 0, N * sizeof(int));

  d_a = (int *)omp_target_alloc(DataSize, DefaultDeviceNum);

  omp_target_associate_ptr(p_a, d_a, DataSize, 0, DefaultDeviceNum);

#pragma omp target update to(p_a[0:N])

  omp_target_disassociate_ptr(p_a, DefaultDeviceNum);

  omp_target_free(d_a, DefaultDeviceNum);
  free(p_a);
}

TEST(DataOpCallbacks, Target_Update) {
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecordDeallocation);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);

  int *p_a = nullptr;
  int *d_a = nullptr;
  int DefaultDeviceNum = omp_get_default_device();
  const int N = 1000;
  auto DataSize = N * sizeof(int);
  p_a = (int *)malloc(DataSize);
  memset(p_a, 0, N * sizeof(int));

  d_a = (int *)omp_target_alloc(DataSize, DefaultDeviceNum);

  omp_target_associate_ptr(p_a, d_a, DataSize, 0, DefaultDeviceNum);

#pragma omp target update to(p_a)

  omp_target_disassociate_ptr(p_a, DefaultDeviceNum);

  omp_target_free(d_a, DefaultDeviceNum);
  free(p_a);
}

TEST(DataOpCallbacks, DISABLED_Target_Update_stack) {
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecordDeallocation);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);

  const int N = 1000;
  int p_a[N];
  int *d_a = nullptr;
  int DefaultDeviceNum = omp_get_default_device();
  auto DataSize = N * sizeof(int);
  // p_a = (int *)malloc(DataSize);
  memset(p_a, 0, N * sizeof(int));

  d_a = (int *)omp_target_alloc(DataSize, DefaultDeviceNum);

  omp_target_associate_ptr(p_a, d_a, DataSize, 0, DefaultDeviceNum);

#pragma omp target update to(p_a)

  omp_target_disassociate_ptr(p_a, DefaultDeviceNum);

  omp_target_free(d_a, DefaultDeviceNum);
}

TEST(DataOpCallbacks, WorkTest) {
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecordDeallocation);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);

  OMPT_PERMIT_EVENT(EventTy::Dispatch);
  OMPT_PERMIT_EVENT(EventTy::Work);

  int N = 100000;
  int i;
  int numThreads=2;

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

#pragma omp parallel for num_threads(numThreads)
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }

#pragma omp barrier

}

TEST(DataOpCallbacks, ExplicitAllocatorAndUpdate_DataOp) {
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecordDeallocation);

  int *p_a = nullptr;
  int *d_a = nullptr;
  int DefaultDeviceNum = omp_get_default_device();
  const int N = 1000;
  auto DataSize = N * sizeof(int);
  p_a = (int *)malloc(DataSize);
  memset(p_a, 0, N * sizeof(int));

  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ALLOC, BEGIN, DataSize);
  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ALLOC, END, DataSize, d_a);
  d_a = (int *)omp_target_alloc(DataSize, DefaultDeviceNum);

  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ASSOCIATE, BEGIN, DataSize, p_a, d_a);
  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, ASSOCIATE, END, DataSize, p_a, d_a);
  omp_target_associate_ptr(p_a, d_a, DataSize, 0, DefaultDeviceNum);

  OMPT_ASSERT_SEQUENCE(TargetEmi, UPDATE, BEGIN, DefaultDeviceNum);
  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, H2D, BEGIN, DataSize, p_a);
  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, H2D, END, DataSize, p_a);
  OMPT_ASSERT_SEQUENCE(TargetEmi, UPDATE, END, DefaultDeviceNum);
#pragma omp target update to(p_a[0:N])

  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DISASSOCIATE, BEGIN, 0, p_a);
  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DISASSOCIATE, END, 0, p_a);
  omp_target_disassociate_ptr(p_a, DefaultDeviceNum);

  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DELETE, BEGIN, 0, d_a);
  OMPT_ASSERT_SEQUENCE(TargetDataOpEmi, DELETE, END, 0, d_a);
  omp_target_free(d_a, DefaultDeviceNum);
  free(p_a);
}

// FIXME: Leave this suite here, so it gets discovered last and executed first.
TEST(InitFiniSuite, DeviceLoad) {
  OMPT_SUPPRESS_EVENT(EventTy::DeviceLoad)
  OMPT_SUPPRESS_EVENT(EventTy::TargetEmi)
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOpEmi)
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmitEmi)
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRecord);

  int N = 128;
  int a[N];

  for (int DeviceNum = 0; DeviceNum < omp_get_num_devices(); ++DeviceNum) {
#pragma omp target parallel for device(DeviceNum)
    {
      for (int j = 0; j < N; j++)
        a[j] = 0;
    }
  }
}

int main(int argc, char **argv) {
  Runner R;
  return R.run();
}
