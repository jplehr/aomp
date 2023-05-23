include ../../Makefile.defs

TESTNAME     = mapGlobalAndPTr
TESTSRC_MAIN = map-global-and-local-ptr.cpp
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

CLANG        ?= clang++
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)
EXTRA_CFLAGS = -fdisable-host-devmem
#-ccc-print-phases
#"-\#\#\#"
RUNENV       = HSA_XNACK=1 OMPX_APU_MAPS=1 LIBOMPTARGET_DEBUG=1

include ../Makefile.rules
