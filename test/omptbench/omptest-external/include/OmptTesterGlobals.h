//===ompTest/include/OmptTesterGlobals.h - omptest global functions --C++===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef OPENMP_LIBOMPTARGET_TEST_OMPTEST_OMPTTESTERGLOBALS_H
#define OPENMP_LIBOMPTARGET_TEST_OMPTEST_OMPTTESTERGLOBALS_H

#include <omp-tools.h>

#ifdef __cplusplus
extern "C" {
#endif
ompt_start_tool_result_t *ompt_start_tool(unsigned int omp_version,
                                          const char *runtime_version);
int start_trace(ompt_device_t *Device);
int flush_trace(ompt_device_t *Device);
// Function which calls flush_trace(ompt_device_t *) on all traced devices.
int flush_traced_devices();
int stop_trace(ompt_device_t *Device);
void libomptest_global_eventreporter_set_active(bool State);
#ifdef __cplusplus
}
#endif

#endif
