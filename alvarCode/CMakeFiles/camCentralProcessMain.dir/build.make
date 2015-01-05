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
include CMakeFiles/camCentralProcessMain.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/camCentralProcessMain.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/camCentralProcessMain.dir/flags.make

CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o: CMakeFiles/camCentralProcessMain.dir/flags.make
CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o: camCentralProcessMain.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nehchal/VisionProjectKrang/alvarCode/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o -c /home/nehchal/VisionProjectKrang/alvarCode/camCentralProcessMain.cpp

CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nehchal/VisionProjectKrang/alvarCode/camCentralProcessMain.cpp > CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.i

CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nehchal/VisionProjectKrang/alvarCode/camCentralProcessMain.cpp -o CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.s

CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.requires:
.PHONY : CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.requires

CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.provides: CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.requires
	$(MAKE) -f CMakeFiles/camCentralProcessMain.dir/build.make CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.provides.build
.PHONY : CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.provides

CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.provides.build: CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o

# Object files for target camCentralProcessMain
camCentralProcessMain_OBJECTS = \
"CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o"

# External object files for target camCentralProcessMain
camCentralProcessMain_EXTERNAL_OBJECTS =

bin/camCentralProcessMain: CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o
bin/camCentralProcessMain: lib/libcamCentralProcess.a
bin/camCentralProcessMain: lib/libglobalStuff.a
bin/camCentralProcessMain: lib/libworldModel.a
bin/camCentralProcessMain: lib/libcvViewer.a
bin/camCentralProcessMain: lib/libglutViewer.a
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libGL.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libSM.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libICE.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libX11.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libXext.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libglut.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libXmu.so
bin/camCentralProcessMain: /usr/lib/x86_64-linux-gnu/libXi.so
bin/camCentralProcessMain: CMakeFiles/camCentralProcessMain.dir/build.make
bin/camCentralProcessMain: CMakeFiles/camCentralProcessMain.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/camCentralProcessMain"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/camCentralProcessMain.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/camCentralProcessMain.dir/build: bin/camCentralProcessMain
.PHONY : CMakeFiles/camCentralProcessMain.dir/build

CMakeFiles/camCentralProcessMain.dir/requires: CMakeFiles/camCentralProcessMain.dir/camCentralProcessMain.cpp.o.requires
.PHONY : CMakeFiles/camCentralProcessMain.dir/requires

CMakeFiles/camCentralProcessMain.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/camCentralProcessMain.dir/cmake_clean.cmake
.PHONY : CMakeFiles/camCentralProcessMain.dir/clean

CMakeFiles/camCentralProcessMain.dir/depend:
	cd /home/nehchal/VisionProjectKrang/alvarCode && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode/CMakeFiles/camCentralProcessMain.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/camCentralProcessMain.dir/depend

