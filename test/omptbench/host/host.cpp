#include <cstring>
#include <stdio.h>
#include "omp.h"

#include "OmptTester.h"

using namespace omptest;
using namespace internal;


TEST(WorkDistribution, SingleParallelFor) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);

  int N = 100000;
  int i;
  int numThreads=4;

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_GENERATE_EVENTS(numThreads-1, OMPT_ASSERT_SET(ThreadBegin, ompt_thread_worker));
#pragma omp parallel for
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }
}

TEST(WorkDistribution, SingleParallelForNumThreadsClause) {
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

  OMPT_ASSERT_SEQUENCE(ParallelBegin, numThreads);
  OMPT_ASSERT_SEQUENCE_SUSPEND();
  OMPT_ASSERT_SEQUENCE(ThreadBegin, ompt_thread_worker);
#pragma omp parallel for num_threads(numThreads)
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }
}

TEST(WorkDistribution, DISABLED_TwoNestedParallelFor) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);

  int N = 100000;
  int i;
  int numThreads=omp_get_num_threads();

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_ASSERT_SET(ThreadBegin, ompt_thread_worker);
#pragma omp parallel for
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }
}

TEST(WorkDistribution, DISABLED_TwoNestedParallelForNumThreadsClause) {
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
    for (int j = 0; j< N; j++) {
      #pragma omp parallel for num_threads(numThreads)
      for (int k = 0; k < N; ++k)
        a[j]=b[j];
    }
  }
}

TEST(WorkDistribution, SingleParallelForWithSingleSection){
  int N = 100000;
  int i, c;

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

#pragma omp parallel
  {
      #pragma omp single
      {
        c = 666;
      }
    #pragma omp for
    for (int j = 0; j< N; j++) {
      a[j] = b[j];
    }
  }
}

TEST(WorkDistribution, SingleParallelWithSingleSection) {
  // TODO: Implement
}

TEST(WorkDistribution, SingleParallelWithScopeSection) {
  // TODO: Implement
}

TEST(WorkDistribution, SingleParallelOneSectionsTwoSection) {
  // TODO: Implement
}

TEST(WorkDistribution, SingleParallelForStaticSchedule) {
  // TODO: Implement
}

TEST(WorkDistribution, SingleParallelForDynamicSchedule) {
  // TODO: Implement
}

TEST(WorkDistribution, SingleParallelForGuidedSchedule) {
  // TODO: Implement
}


int main(int argc, char **argv) {
  Runner R;
  R.run();

  return 0;
}
