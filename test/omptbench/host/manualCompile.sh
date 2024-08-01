#!/usr/bin/env bash

which clang++

OMPTEST_BASEDIR=$(realpath -s "$(pwd)/../omptest-external")
OMPTEST_INCLUDE=${OMPTEST_BASEDIR}/include
OMPTEST_LIB_L=${OMPTEST_BASEDIR}/build
OMPTEST_LIB_N='-lomptest'
INPUT_N='host.cpp'
OUTPUT_N='omptbench-host.exe'

CFLAGS="-v -fopenmp -O2 -I${OMPTEST_INCLUDE}"
LDFLAGS="-L${OMPTEST_LIB_L} ${OMPTEST_LIB_N}"
LDFLAGS+=" -Wl,--disable-new-dtags -rpath ${OMPTEST_LIB_L}"

export LIBRARY_PATH=${OMPTEST_LIB_L}:${LIBRARY_PATH}
clang++ ${CFLAGS} ${INPUT_N} -o ${OUTPUT_N} ${LDFLAGS}

# Run using:
# EC=$?
# if [ $EC -ne 0 ]; then
#   exit $EC
# fi
# export OMPTEST_USE_OMPT_EMI=0
# export OMPTEST_USE_OMPT_TRACING=0
# export OMPTEST_RUN_AS_TESTSUITE=1
# ./${OUTPUT_N}
