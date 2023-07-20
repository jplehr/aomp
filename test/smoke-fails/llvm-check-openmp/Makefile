include ../../Makefile.defs

OPENMP_BUILD_DIR=$(AOMP_REPOS)/build/openmp

TESTNAME     = llvm-check-openmp
TESTSRC_MAIN = main.c
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

HERE = $(PWD)
CHECK_OPENMP = $(shell cd $(OPENMP_BUILD_DIR) && ninja -j 32 check-openmp >$(HERE)/check-openmp.log 2>&1 && echo $?)
RUNCMD = exit $(CHECK_OPENMP)

CLANG        ?= clang
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)
#-ccc-print-phases
#"-\#\#\#"

include ../Makefile.rules
