#!/usr/bin/env bash

which clang++

OMPTEST_BASEDIR=$(realpath -s "$(pwd)/../../omptest-external")
OMPTEST_INCLUDE=${OMPTEST_BASEDIR}/include
OMPTEST_LIB_L=${OMPTEST_BASEDIR}/build
OMPTEST_LIB_N='-lomptest'
INPUT_N='omptbench-device-emi.cpp'
OUTPUT_N='omptbench-device-emi.exe'

INSTALLED_GPU=$(amdgpu-arch | grep -m 1 -E gfx[^0]{1}.{2})
OFFLOAD_ARCH="--offload-arch=${INSTALLED_GPU}"

CFLAGS="-v -O2 -fopenmp ${OFFLOAD_ARCH} -I${OMPTEST_INCLUDE}"
LDFLAGS="-L${OMPTEST_LIB_L} ${OMPTEST_LIB_N}"
LDFLAGS+=" -Wl,--disable-new-dtags -rpath ${OMPTEST_LIB_L}"

export LIBRARY_PATH=${OMPTEST_LIB_L}:${LIBRARY_PATH}
clang++ ${CFLAGS} ${INPUT_N} -o ${OUTPUT_N} ${LDFLAGS}

# Run using:
# export OMPTEST_USE_OMPT_EMI=1
# export OMPTEST_USE_OMPT_TRACING=1
# export OMPTEST_RUN_AS_TESTSUITE=1
# ./${OUTPUT_N}
