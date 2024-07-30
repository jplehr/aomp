//===ompTest/include/OmptTester.h - omptest main include file --C++===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef OPENMP_LIBOMPTARGET_TEST_OMPTEST_OMPTTESTER_H
#define OPENMP_LIBOMPTARGET_TEST_OMPTEST_OMPTTESTER_H

#include "AssertMacros.h"
#include "OmptAliases.h"
#include "OmptAssertEvent.h"
#include "OmptAsserter.h"
#include "OmptCallbackHandler.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#ifdef LIBOFFLOAD_LIBOMPTEST_USE_GOOGLETEST
#include "OmptTesterGoogleTest.h"
#else
#include "OmptTesterStandalone.h"
#endif

#endif
