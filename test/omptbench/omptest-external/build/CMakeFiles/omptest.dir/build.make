# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build

# Include any dependencies generated for this target.
include CMakeFiles/omptest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/omptest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/omptest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/omptest.dir/flags.make

CMakeFiles/omptest.dir/src/InternalEvent.cpp.o: CMakeFiles/omptest.dir/flags.make
CMakeFiles/omptest.dir/src/InternalEvent.cpp.o: ../src/InternalEvent.cpp
CMakeFiles/omptest.dir/src/InternalEvent.cpp.o: CMakeFiles/omptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/omptest.dir/src/InternalEvent.cpp.o"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/omptest.dir/src/InternalEvent.cpp.o -MF CMakeFiles/omptest.dir/src/InternalEvent.cpp.o.d -o CMakeFiles/omptest.dir/src/InternalEvent.cpp.o -c /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/InternalEvent.cpp

CMakeFiles/omptest.dir/src/InternalEvent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/omptest.dir/src/InternalEvent.cpp.i"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/InternalEvent.cpp > CMakeFiles/omptest.dir/src/InternalEvent.cpp.i

CMakeFiles/omptest.dir/src/InternalEvent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/omptest.dir/src/InternalEvent.cpp.s"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/InternalEvent.cpp -o CMakeFiles/omptest.dir/src/InternalEvent.cpp.s

CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o: CMakeFiles/omptest.dir/flags.make
CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o: ../src/InternalEventOperators.cpp
CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o: CMakeFiles/omptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o -MF CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o.d -o CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o -c /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/InternalEventOperators.cpp

CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.i"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/InternalEventOperators.cpp > CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.i

CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.s"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/InternalEventOperators.cpp -o CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.s

CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o: CMakeFiles/omptest.dir/flags.make
CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o: ../src/OmptAsserter.cpp
CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o: CMakeFiles/omptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o -MF CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o.d -o CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o -c /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptAsserter.cpp

CMakeFiles/omptest.dir/src/OmptAsserter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/omptest.dir/src/OmptAsserter.cpp.i"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptAsserter.cpp > CMakeFiles/omptest.dir/src/OmptAsserter.cpp.i

CMakeFiles/omptest.dir/src/OmptAsserter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/omptest.dir/src/OmptAsserter.cpp.s"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptAsserter.cpp -o CMakeFiles/omptest.dir/src/OmptAsserter.cpp.s

CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o: CMakeFiles/omptest.dir/flags.make
CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o: ../src/OmptAssertEvent.cpp
CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o: CMakeFiles/omptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o -MF CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o.d -o CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o -c /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptAssertEvent.cpp

CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.i"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptAssertEvent.cpp > CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.i

CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.s"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptAssertEvent.cpp -o CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.s

CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o: CMakeFiles/omptest.dir/flags.make
CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o: ../src/OmptCallbackHandler.cpp
CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o: CMakeFiles/omptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o -MF CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o.d -o CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o -c /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptCallbackHandler.cpp

CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.i"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptCallbackHandler.cpp > CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.i

CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.s"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptCallbackHandler.cpp -o CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.s

CMakeFiles/omptest.dir/src/OmptTester.cpp.o: CMakeFiles/omptest.dir/flags.make
CMakeFiles/omptest.dir/src/OmptTester.cpp.o: ../src/OmptTester.cpp
CMakeFiles/omptest.dir/src/OmptTester.cpp.o: CMakeFiles/omptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/omptest.dir/src/OmptTester.cpp.o"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/omptest.dir/src/OmptTester.cpp.o -MF CMakeFiles/omptest.dir/src/OmptTester.cpp.o.d -o CMakeFiles/omptest.dir/src/OmptTester.cpp.o -c /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptTester.cpp

CMakeFiles/omptest.dir/src/OmptTester.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/omptest.dir/src/OmptTester.cpp.i"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptTester.cpp > CMakeFiles/omptest.dir/src/OmptTester.cpp.i

CMakeFiles/omptest.dir/src/OmptTester.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/omptest.dir/src/OmptTester.cpp.s"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptTester.cpp -o CMakeFiles/omptest.dir/src/OmptTester.cpp.s

CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o: CMakeFiles/omptest.dir/flags.make
CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o: ../src/OmptTesterStandalone.cpp
CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o: CMakeFiles/omptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o -MF CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o.d -o CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o -c /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptTesterStandalone.cpp

CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.i"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptTesterStandalone.cpp > CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.i

CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.s"
	/CAR/aomp_19.0-2/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/src/OmptTesterStandalone.cpp -o CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.s

# Object files for target omptest
omptest_OBJECTS = \
"CMakeFiles/omptest.dir/src/InternalEvent.cpp.o" \
"CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o" \
"CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o" \
"CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o" \
"CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o" \
"CMakeFiles/omptest.dir/src/OmptTester.cpp.o" \
"CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o"

# External object files for target omptest
omptest_EXTERNAL_OBJECTS =

libomptest.so: CMakeFiles/omptest.dir/src/InternalEvent.cpp.o
libomptest.so: CMakeFiles/omptest.dir/src/InternalEventOperators.cpp.o
libomptest.so: CMakeFiles/omptest.dir/src/OmptAsserter.cpp.o
libomptest.so: CMakeFiles/omptest.dir/src/OmptAssertEvent.cpp.o
libomptest.so: CMakeFiles/omptest.dir/src/OmptCallbackHandler.cpp.o
libomptest.so: CMakeFiles/omptest.dir/src/OmptTester.cpp.o
libomptest.so: CMakeFiles/omptest.dir/src/OmptTesterStandalone.cpp.o
libomptest.so: CMakeFiles/omptest.dir/build.make
libomptest.so: CMakeFiles/omptest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX shared library libomptest.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/omptest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/omptest.dir/build: libomptest.so
.PHONY : CMakeFiles/omptest.dir/build

CMakeFiles/omptest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/omptest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/omptest.dir/clean

CMakeFiles/omptest.dir/depend:
	cd /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build /home/mhalkenh/git/hpcbugs_aomp/test/omptbench/omptest-external/build/CMakeFiles/omptest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/omptest.dir/depend

