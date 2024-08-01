#include <cstring>
#include <stdio.h>
#include "omp.h"

#include "OmptTester.h"

using namespace omptest;
using namespace internal;

TEST(WorkDistribution, InitRuntime){
  int numThreads = omp_get_num_threads();
  #pragma omp parallel
  {}
}

TEST(WorkDistribution, SingleParallelFor) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);


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
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads-3, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, END));
  // OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL_WORKSHARE, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  // OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL_PARALLEL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);
#pragma omp parallel for
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }
}

TEST(WorkDistribution, SingleParallelForNumThreadsClause) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);

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
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);
#pragma omp parallel for num_threads(numThreads)
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }
}

TEST(WorkDistribution, SingleParallelForWithSingleSection){
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);

  int N = 100000;
  int i, c;
  int numThreads = 4;

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(1, OMPT_ASSERT_SET(Work, WORK_EXEC, BEGIN));
  OMPT_GENERATE_EVENTS(1, OMPT_ASSERT_SET(Work, WORK_EXEC, END));
  OMPT_GENERATE_EVENTS(numThreads-1, OMPT_ASSERT_SET(Work, WORK_SINGLE, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads-1, OMPT_ASSERT_SET(Work, WORK_SINGLE, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);
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

TEST(WorkDistribution, SingleParallelWithScopeSection) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);

  int N = 100000;
  int i, c;
  int numThreads = 4;

  int a[N];
  int b[N];

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_SCOPE, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_SCOPE, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);
#pragma omp parallel
  {
    // FIXME: ICE in AOMP 18.0-1
      // #pragma omp scope
      {
        c = 666;
      }
    #pragma omp for
    for (int j = 0; j< N; j++) {
      a[j] = b[j];
    }
  }
}

TEST(WorkDistribution, SingleParallelOneSectionsTwoSection) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);

  int numThreads=4;

  int N = 100;
  int a[100];
  int b[100];

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_SECT, BEGIN));
  /// TODO: Here we would have dispatch Callbacks with ompt_dispatch_section kind
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_SECT, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        for(int i = 0; i < N; ++i) {
          a[i] = 1;
        }
      }
      #pragma omp section
      {
        for(int i = 0; i < N; ++i) {
          b[i] = 2;
        }
      }
    }
  }
}

TEST(WorkDistribution, SingleParallelForStaticSchedule) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);

  int numThreads=4;

  int N = 100;
  int a[100];
  int b[100] = {0};

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP_STA, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP_STA, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);

  #pragma omp parallel for schedule(static)
  {
    for (int j = 0; j < N; j++)
      a[j]=b[j] + 1;
  }

}

TEST(WorkDistribution, SingleParallelForDynamicSchedule) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);

  int numThreads=4;

  int N = 100;
  int a[100];
  int b[100] = {0};

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP_DYN, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP_DYN, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);

  #pragma omp parallel for schedule(dynamic)
  {
    for (int j = 0; j < N; j++)
      a[j]=b[j] + 1;
  }
}

TEST(WorkDistribution, SingleParallelForGuidedSchedule) {
  OMPT_PERMIT_EVENT(EventTy::ThreadBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelBegin);
  OMPT_PERMIT_EVENT(EventTy::ParallelEnd);
  OMPT_PERMIT_EVENT(EventTy::ImplicitTask);
  OMPT_PERMIT_EVENT(EventTy::Work);
  OMPT_PERMIT_EVENT(EventTy::SyncRegion);

  int numThreads=4;

  int N = 100;
  int a[100];
  int b[100] = {0};

  OMPT_ASSERT_SET(ParallelBegin, numThreads);
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP_GUI, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(Work, WORK_LOOP_GUI, END));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(SyncRegion, SR_BARRIER_IMPL, BEGIN));
  OMPT_GENERATE_EVENTS(numThreads, OMPT_ASSERT_SET(ImplicitTask, END));
  OMPT_ASSERT_SET(ParallelEnd, nullptr);

  #pragma omp parallel for schedule(guided) num_threads(numThreads)
  {
    for (int j = 0; j < N; j++)
      a[j]=b[j] + 1;
  }
}

int main(int argc, char **argv) {
  Runner R;
  R.run();

  return 0;
}
