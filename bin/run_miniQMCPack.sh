#! /usr/bin/env bash

#
# run_miniQMCPack.sh - runs several MiniQMCPack binaries
#
#

# --- Start standard header to set AOMP environment variables ----
realpath=`realpath $0`
thisdir=`dirname $realpath`
. $thisdir/aomp_common_vars
# --- end standard header ----

# Default ROCm installation
: ${ROCM_INSTALL_PATH:=/opt/rocm-5.3.0}

# Control how many OpenMP threads are used by MiniQMCPack
: ${MQMC_OMP_THREADS_TO_USE:=64}

export PATH=$AOMP/bin:$PATH
#export PATH=/home/janplehr/rocm/trunk/bin:$PATH

export hsaruntime64_DIR=${ROCM_INSTALL_PATH}/lib/cmake/hsa-runtime64/
export hipblas_DIR=${ROCM_INSTALL_PATH}/lib/cmake/hipblas/
export hip_DIR=${ROCM_INSTALL_PATH}/lib/cmake/hip
export AMDDeviceLibs_DIR=${ROCM_INSTALL_PATH}/lib/cmake/AMDDeviceLibs/
export amd_comgr_DIR=${ROCM_INSTALL_PATH}/lib/cmake/amd_comgr/
export rocblas_DIR=${ROCM_INSTALL_PATH}/lib/cmake/rocblas/
export rocsolver_DIR=${ROCM_INSTALL_PATH}/lib/cmake/rocsolver/

# Set the default build prefix, i.e., build-top-level
: ${MQMCPACK_BUILD_PREFIX:=~/miniqmc_build}
# Set the default build directory name
: ${MQMCPACK_BUILD_DIR:=${MQMCPACK_BUILD_PREFIX}/build_aomp_clang}
# Path to the miniqmc source directory
: ${MQMC_SOURCE_DIR:=.}
# how many threads should be used for building miniqmc
: ${MQMC_NUM_BUILD_PROCS:=32}

rm -rf ${MQMCPACK_BUILD_DIR}
CMAKE_PREFIX_PATH=${ROCM_INSTALL_PATH}/lib/cmake/ cmake -B ${MQMCPACK_BUILD_DIR} -S ${MQMC_SOURCE_DIR} -DCMAKE_CXX_COMPILER=clang++ -DENABLE_OFFLOAD=ON -DQMC_ENABLE_ROCM=ON -DCMAKE_CXX_FLAGS='' -DAMDGPU_DISABLE_HOST_DEVMEM=ON -DCMAKE_VERBOSE_MAKEFILE=ON
#CMAKE_PREFIX_PATH=${ROCM_INSTALL_PATH}/lib/cmake/ cmake -B ${MQMCPACK_BUILD_DIR} -S ${MQMC_SOURCE_DIR} -DCMAKE_CXX_COMPILER=clang++ -DENABLE_OFFLOAD=ON -DQMC_ENABLE_ROCM=ON -DAMDGPU_DISABLE_HOST_DEVMEM=OFF -DCMAKE_VERBOSE_MAKEFILE=ON
# -DCLANGRT_BUILTINS=/home/janplehr/rocm/trunk/lib/clang/17/lib/x86_64-unknown-linux-gnu/libclang_rt.builtins.a

cmake --build ${MQMCPACK_BUILD_DIR}  --clean-first -j ${MQMC_NUM_BUILD_PROCS}

echo "Running Tests"
echo "OMP_NUM_THREADS=${MQMC_OMP_THREADS_TO_USE} ${MQMCPACK_BUILD_DIR}/bin/check_spo_batched_reduction -n 10"
OMP_NUM_THREADS=${MQMC_OMP_THREADS_TO_USE} ${MQMCPACK_BUILD_DIR}/bin/check_spo_batched_reduction -n 10

echo ""
echo "OMP_NUM_THREADS=${MQMC_OMP_THREADS_TO_USE} ${MQMCPACK_BUILD_DIR}/bin/miniqmc"
OMP_NUM_THREADS=${MQMC_OMP_THREADS_TO_USE} ${MQMCPACK_BUILD_DIR}/bin/miniqmc -v

echo ""
echo "OMP_NUM_THREADS=${MQMC_OMP_THREADS_TO_USE} ${MQMCPACK_BUILD_DIR}/bin/check_spo -n 10"
OMP_NUM_THREADS=${MQMC_OMP_THREADS_TO_USE} ${MQMCPACK_BUILD_DIR}/bin/check_spo -n 10 -v
