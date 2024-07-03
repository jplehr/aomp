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

TEST(VeccopyTraces, OnDevice_one) {

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
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
  #pragma omp target parallel for
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
}

TEST(VeccopyTraces, OnDevice_two) {

  OMPT_SUPPRESS_EVENT(EventTy::Target);
  OMPT_SUPPRESS_EVENT(EventTy::TargetSubmit);
  OMPT_SUPPRESS_EVENT(EventTy::TargetDataOp);
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
  #pragma omp target teams distribute parallel for nowait
  {
    for (int j = 0; j < N; ++j)
      a[j] = b[j];
  }
  OMPT_ASSERT_SYNC_POINT("Final sync point");
}


int main(int argc, char **argv) {
  Runner R;
  R.run();

  return 0;
}
