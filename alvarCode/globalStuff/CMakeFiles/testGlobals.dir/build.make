# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nehchal/VisionProjectKrang/alvarCode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nehchal/VisionProjectKrang/alvarCode

# Include any dependencies generated for this target.
include globalStuff/CMakeFiles/testGlobals.dir/depend.make

# Include the progress variables for this target.
include globalStuff/CMakeFiles/testGlobals.dir/progress.make

# Include the compile flags for this target's objects.
include globalStuff/CMakeFiles/testGlobals.dir/flags.make

globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o: globalStuff/CMakeFiles/testGlobals.dir/flags.make
globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o: globalStuff/globalTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nehchal/VisionProjectKrang/alvarCode/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o"
	cd /home/nehchal/VisionProjectKrang/alvarCode/globalStuff && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/testGlobals.dir/globalTest.cpp.o -c /home/nehchal/VisionProjectKrang/alvarCode/globalStuff/globalTest.cpp

globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testGlobals.dir/globalTest.cpp.i"
	cd /home/nehchal/VisionProjectKrang/alvarCode/globalStuff && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nehchal/VisionProjectKrang/alvarCode/globalStuff/globalTest.cpp > CMakeFiles/testGlobals.dir/globalTest.cpp.i

globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testGlobals.dir/globalTest.cpp.s"
	cd /home/nehchal/VisionProjectKrang/alvarCode/globalStuff && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nehchal/VisionProjectKrang/alvarCode/globalStuff/globalTest.cpp -o CMakeFiles/testGlobals.dir/globalTest.cpp.s

globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.requires:
.PHONY : globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.requires

globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.provides: globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.requires
	$(MAKE) -f globalStuff/CMakeFiles/testGlobals.dir/build.make globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.provides.build
.PHONY : globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.provides

globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.provides.build: globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o

# Object files for target testGlobals
testGlobals_OBJECTS = \
"CMakeFiles/testGlobals.dir/globalTest.cpp.o"

# External object files for target testGlobals
testGlobals_EXTERNAL_OBJECTS =

globalStuff/testGlobals: globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o
globalStuff/testGlobals: lib/libglobalStuff.a
globalStuff/testGlobals: globalStuff/CMakeFiles/testGlobals.dir/build.make
globalStuff/testGlobals: globalStuff/CMakeFiles/testGlobals.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable testGlobals"
	cd /home/nehchal/VisionProjectKrang/alvarCode/globalStuff && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testGlobals.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
globalStuff/CMakeFiles/testGlobals.dir/build: globalStuff/testGlobals
.PHONY : globalStuff/CMakeFiles/testGlobals.dir/build

globalStuff/CMakeFiles/testGlobals.dir/requires: globalStuff/CMakeFiles/testGlobals.dir/globalTest.cpp.o.requires
.PHONY : globalStuff/CMakeFiles/testGlobals.dir/requires

globalStuff/CMakeFiles/testGlobals.dir/clean:
	cd /home/nehchal/VisionProjectKrang/alvarCode/globalStuff && $(CMAKE_COMMAND) -P CMakeFiles/testGlobals.dir/cmake_clean.cmake
.PHONY : globalStuff/CMakeFiles/testGlobals.dir/clean

globalStuff/CMakeFiles/testGlobals.dir/depend:
	cd /home/nehchal/VisionProjectKrang/alvarCode && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode/globalStuff /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode/globalStuff /home/nehchal/VisionProjectKrang/alvarCode/globalStuff/CMakeFiles/testGlobals.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : globalStuff/CMakeFiles/testGlobals.dir/depend

