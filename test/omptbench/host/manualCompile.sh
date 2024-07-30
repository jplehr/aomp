#!/usr/bin/env bash

which clang++

OMPTEST_LIB_L=../omptest-external/build
OMPTEST_LIB_N='-lomptest -lomp'
OMPTEST_INCLUDE=../omptest-external/include
echo "clang++ -v -fopenmp -O2 -o manually-compiled.exe -I${OMPTEST_INCLUDE} -L${OMPTEST_LIB_L}  host.cpp ${OMPTEST_LIB_N}"
clang++ -v -fopenmp -O2 -o manually-compiled.exe -I${OMPTEST_INCLUDE} -L${OMPTEST_LIB_L}  host.cpp ${OMPTEST_LIB_N}
