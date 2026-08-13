include ../../Makefile.defs

TESTNAME     = spirv_parallel_for
TESTSRC_MAIN = spirv_parallel_for.c
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

# SPIR-V only test
SUPPORTED = amdgcnspirv

CLANG        ?= clang
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)

include ../Makefile.rules
