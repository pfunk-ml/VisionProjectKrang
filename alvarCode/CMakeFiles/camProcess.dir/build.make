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
include CMakeFiles/camProcess.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/camProcess.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/camProcess.dir/flags.make

CMakeFiles/camProcess.dir/camProcess.cpp.o: CMakeFiles/camProcess.dir/flags.make
CMakeFiles/camProcess.dir/camProcess.cpp.o: camProcess.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nehchal/VisionProjectKrang/alvarCode/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/camProcess.dir/camProcess.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/camProcess.dir/camProcess.cpp.o -c /home/nehchal/VisionProjectKrang/alvarCode/camProcess.cpp

CMakeFiles/camProcess.dir/camProcess.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/camProcess.dir/camProcess.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nehchal/VisionProjectKrang/alvarCode/camProcess.cpp > CMakeFiles/camProcess.dir/camProcess.cpp.i

CMakeFiles/camProcess.dir/camProcess.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/camProcess.dir/camProcess.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nehchal/VisionProjectKrang/alvarCode/camProcess.cpp -o CMakeFiles/camProcess.dir/camProcess.cpp.s

CMakeFiles/camProcess.dir/camProcess.cpp.o.requires:
.PHONY : CMakeFiles/camProcess.dir/camProcess.cpp.o.requires

CMakeFiles/camProcess.dir/camProcess.cpp.o.provides: CMakeFiles/camProcess.dir/camProcess.cpp.o.requires
	$(MAKE) -f CMakeFiles/camProcess.dir/build.make CMakeFiles/camProcess.dir/camProcess.cpp.o.provides.build
.PHONY : CMakeFiles/camProcess.dir/camProcess.cpp.o.provides

CMakeFiles/camProcess.dir/camProcess.cpp.o.provides.build: CMakeFiles/camProcess.dir/camProcess.cpp.o

# Object files for target camProcess
camProcess_OBJECTS = \
"CMakeFiles/camProcess.dir/camProcess.cpp.o"

# External object files for target camProcess
camProcess_EXTERNAL_OBJECTS =

bin/camProcess: CMakeFiles/camProcess.dir/camProcess.cpp.o
bin/camProcess: lib/libglobalStuff.a
bin/camProcess: lib/libcvViewer.a
bin/camProcess: lib/libglutViewer.a
bin/camProcess: /usr/local/lib/libopencv_videostab.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_video.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_ts.a
bin/camProcess: /usr/local/lib/libopencv_superres.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_stitching.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_photo.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_ocl.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_objdetect.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_nonfree.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_ml.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_legacy.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_imgproc.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_highgui.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_gpu.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_flann.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_features2d.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_core.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_contrib.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_calib3d.so.2.4.8
bin/camProcess: /usr/lib/x86_64-linux-gnu/libglut.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libXmu.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libXi.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libGL.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libSM.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libICE.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libX11.so
bin/camProcess: /usr/lib/x86_64-linux-gnu/libXext.so
bin/camProcess: /usr/local/lib/libopencv_nonfree.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_ocl.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_gpu.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_photo.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_objdetect.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_legacy.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_video.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_ml.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_calib3d.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_features2d.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_highgui.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_imgproc.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_flann.so.2.4.8
bin/camProcess: /usr/local/lib/libopencv_core.so.2.4.8
bin/camProcess: CMakeFiles/camProcess.dir/build.make
bin/camProcess: CMakeFiles/camProcess.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/camProcess"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/camProcess.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/camProcess.dir/build: bin/camProcess
.PHONY : CMakeFiles/camProcess.dir/build

CMakeFiles/camProcess.dir/requires: CMakeFiles/camProcess.dir/camProcess.cpp.o.requires
.PHONY : CMakeFiles/camProcess.dir/requires

CMakeFiles/camProcess.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/camProcess.dir/cmake_clean.cmake
.PHONY : CMakeFiles/camProcess.dir/clean

CMakeFiles/camProcess.dir/depend:
	cd /home/nehchal/VisionProjectKrang/alvarCode && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode/CMakeFiles/camProcess.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/camProcess.dir/depend

