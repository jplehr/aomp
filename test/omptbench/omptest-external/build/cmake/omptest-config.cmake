################################################################################
##
## libomptest cmake configuration file.
## Enable support for find_package.
##
################################################################################

# Compute installation prefix relative to this file.
get_filename_component(_DIR_CURRENT   "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_DIR_AOMP_ROOT "${_DIR_CURRENT}/../../.."   ABSOLUTE)
get_filename_component(_DIR_AOMP_LIB  "${_DIR_AOMP_ROOT}/lib"      ABSOLUTE)
get_filename_component(_DIR_AOMP_BIN  "${_DIR_AOMP_ROOT}/bin"      ABSOLUTE)

# Provide header and library paths.
set(omptest_INCLUDE_DIR   "${_DIR_AOMP_LIB}/omptest/include")
set(omptest_LIBRARY_ROOT  "${_DIR_AOMP_LIB}")

# Provide reasonable compiler default values.
set(omptest_C_COMPILER    "${_DIR_AOMP_BIN}/clang")
set(omptest_CXX_COMPILER  "${_DIR_AOMP_BIN}/clang++")
