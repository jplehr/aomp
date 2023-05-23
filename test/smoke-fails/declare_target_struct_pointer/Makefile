include ../../Makefile.defs

TESTNAME     = declare_target_struct_pointer
TESTSRC_MAIN = declare_target_struct_pointer.cpp
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

CLANG        ?= clang++
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)
EXTRA_CFLAGS = -fdisable-host-devmem -save-temps
#-ccc-print-phases
#"-\#\#\#"

include ../Makefile.rules
