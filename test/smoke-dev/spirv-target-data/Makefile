include ../../Makefile.defs

TESTNAME     = spirv_target_data
TESTSRC_MAIN = spirv_target_data.c
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

# SPIR-V only test
SUPPORTED = amdgcnspirv

CLANG        ?= clang
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)

include ../Makefile.rules
