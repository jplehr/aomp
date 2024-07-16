#include "omp.h"

#include "OmptTester.h"

using namespace omptest;
using namespace internal;

TEST(VeccopyTraces, OnHost_one) {

  int N = 10;
  int a[N];

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
  OMPT_PERMIT_EVENT(EventTy::BufferRecord);

  OMPT_ASSERT_SET_NOT(BufferRecord, CB_TARGET, TARGET, BEGIN);
  OMPT_ASSERT_SET_NOT(BufferRecord, CB_DATAOP, ALLOC, N * sizeof(int));

  #pragma omp parallel for
  {
    for (int i=0; i < N; ++i) {
      a[i] = i;
    }
  }
}

TEST(VeccopyTraces, OnDeviceBufferRecord_parallel_for) {

  OMPT_SUPPRESS_EVENT(EventTy::DeviceLoad);
  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);
  OMPT_PERMIT_EVENT(EventTy::BufferRecord);
  OMPT_ASSERT_SET_MODE_STRICT();
  const int N = 10000;
  int a[N];
  int b[N];

  for (int i=0; i<N; i++)
    a[i]=0;

  for (int i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, BEGIN);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_KERNEL);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, END);
  #pragma omp target parallel for map(a, b)
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
  OMPT_ASSERT_SYNC_POINT("Final sync point");
}

TEST(VeccopyTraces, OnDeviceCallbacks_parallel_for) {

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
  OMPT_PERMIT_EVENT(EventTy::BufferRecord);
  OMPT_ASSERT_SET_MODE_RELAXED();
  const int N = 10000;
  int a[N];
  int b[N];

  for (int i=0; i<N; i++)
    a[i]=0;

  for (int i=0; i<N; i++)
    b[i]=i;

  // This works with force-sync-regions, but not in default async execution.
  // Is this a bug in our implementation.
  OMPT_GENERATE_EVENTS(5, OMPT_ASSERT_SET(BufferRequest, 0, nullptr, 0));
  OMPT_GENERATE_EVENTS(11, OMPT_ASSERT_SET(BufferComplete, 0, nullptr, 0, 0, false));
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, BEGIN);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_KERNEL);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, END);
  #pragma omp target parallel for map(a, b)
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
  OMPT_ASSERT_SYNC_POINT("Final sync point");
}

TEST(VeccopyTraces, OnDeviceBufferRecord_teams_distribute_parallel_for) {

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);
  OMPT_PERMIT_EVENT(EventTy::BufferRecord);

  const int N = 10000;
  int a[N];
  int b[N];

  for (int i=0; i<N; i++)
    a[i]=0;

  for (int i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, BEGIN);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_KERNEL);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, END);
  #pragma omp target teams distribute parallel for map(a, b)
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
  OMPT_ASSERT_SYNC_POINT("Final sync point");
}

TEST(VeccopyTraces, OnDeviceCallbacks_teams_distribute_parallel_for) {

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
  OMPT_PERMIT_EVENT(EventTy::BufferRecord);
  OMPT_ASSERT_SET_MODE_RELAXED();

  const int N = 10000;
  int a[N];
  int b[N];

  for (int i=0; i<N; i++)
    a[i]=0;

  for (int i=0; i<N; i++)
    b[i]=i;

  // 2 data transfers TO device, 1 kernel, 2 data transfers FROM device
  OMPT_GENERATE_EVENTS(5, OMPT_ASSERT_SET(BufferRequest, 0, nullptr, 0));
  OMPT_GENERATE_EVENTS(11, OMPT_ASSERT_SET(BufferComplete, 0, nullptr, 0, 0, false));
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, BEGIN);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_KERNEL);
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, END);
  #pragma omp target teams distribute parallel for map(a, b)
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
}

TEST(VeccopyTraces, OnDeviceBufferRecord_teams_distribute_parallel_for_nowait) {

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
  OMPT_SUPPRESS_EVENT(EventTy::BufferRequest);
  OMPT_SUPPRESS_EVENT(EventTy::BufferComplete);
  OMPT_PERMIT_EVENT(EventTy::BufferRecord);

  const int N = 10000;
  int a[N];
  int b[N];

  for (int i=0; i<N; i++)
    a[i]=0;

  for (int i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, BEGIN);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_KERNEL);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, END);
  #pragma omp target teams distribute parallel for nowait map(a, b)
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
  OMPT_ASSERT_SYNC_POINT("Final sync point");
}

TEST(VeccopyTraces, OnDeviceCallbacks_teams_distribute_parallel_for_nowait) {

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
  OMPT_PERMIT_EVENT(EventTy::BufferRecord);
  OMPT_ASSERT_SET_MODE_RELAXED();

  const int N = 10000;
  int a[N];
  int b[N];

  for (int i=0; i<N; i++)
    a[i]=0;

  for (int i=0; i<N; i++)
    b[i]=i;

  // 2 data transfers TO device, 1 kernel, 2 data transfers FROM device
  OMPT_GENERATE_EVENTS(5, OMPT_ASSERT_SET(BufferRequest, 0, nullptr, 0));
  OMPT_GENERATE_EVENTS(11, OMPT_ASSERT_SET(BufferComplete, 0, nullptr, 0, 0, false));
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, BEGIN);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_DATAOP);
  OMPT_ASSERT_SET(BufferRecord, CB_KERNEL);
  OMPT_ASSERT_SET(BufferRecord, CB_TARGET, TARGET, END);
  #pragma omp target teams distribute parallel for nowait map(a, b)
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
}


int main(int argc, char **argv) {
  Runner R;
  R.run();

  return 0;
}
