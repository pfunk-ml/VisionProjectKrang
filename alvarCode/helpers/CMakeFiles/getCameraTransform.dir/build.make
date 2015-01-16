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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nehchal/VisionProjectKrang/alvarCode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nehchal/VisionProjectKrang/alvarCode

# Include any dependencies generated for this target.
include helpers/CMakeFiles/getCameraTransform.dir/depend.make

# Include the progress variables for this target.
include helpers/CMakeFiles/getCameraTransform.dir/progress.make

# Include the compile flags for this target's objects.
include helpers/CMakeFiles/getCameraTransform.dir/flags.make

helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o: helpers/CMakeFiles/getCameraTransform.dir/flags.make
helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o: helpers/getCameraTransform.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nehchal/VisionProjectKrang/alvarCode/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o"
	cd /home/nehchal/VisionProjectKrang/alvarCode/helpers && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o -c /home/nehchal/VisionProjectKrang/alvarCode/helpers/getCameraTransform.cpp

helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.i"
	cd /home/nehchal/VisionProjectKrang/alvarCode/helpers && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nehchal/VisionProjectKrang/alvarCode/helpers/getCameraTransform.cpp > CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.i

helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.s"
	cd /home/nehchal/VisionProjectKrang/alvarCode/helpers && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nehchal/VisionProjectKrang/alvarCode/helpers/getCameraTransform.cpp -o CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.s

helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.requires:
.PHONY : helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.requires

helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.provides: helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.requires
	$(MAKE) -f helpers/CMakeFiles/getCameraTransform.dir/build.make helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.provides.build
.PHONY : helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.provides

helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.provides.build: helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o

# Object files for target getCameraTransform
getCameraTransform_OBJECTS = \
"CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o"

# External object files for target getCameraTransform
getCameraTransform_EXTERNAL_OBJECTS =

bin/getCameraTransform: helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o
bin/getCameraTransform: lib/libglobalStuff.a
bin/getCameraTransform: lib/libcvViewer.a
bin/getCameraTransform: lib/libglutViewer.a
bin/getCameraTransform: /usr/local/lib/libopencv_videostab.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_video.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_ts.a
bin/getCameraTransform: /usr/local/lib/libopencv_superres.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_stitching.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_photo.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_ocl.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_objdetect.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_nonfree.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_ml.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_legacy.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_imgproc.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_highgui.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_gpu.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_flann.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_features2d.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_core.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_contrib.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_calib3d.so.2.4.10
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libglut.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libXmu.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libXi.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libGL.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libSM.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libICE.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libX11.so
bin/getCameraTransform: /usr/lib/x86_64-linux-gnu/libXext.so
bin/getCameraTransform: /usr/local/lib/libopencv_nonfree.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_ocl.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_gpu.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_photo.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_objdetect.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_legacy.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_video.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_ml.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_calib3d.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_features2d.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_highgui.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_imgproc.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_flann.so.2.4.10
bin/getCameraTransform: /usr/local/lib/libopencv_core.so.2.4.10
bin/getCameraTransform: helpers/CMakeFiles/getCameraTransform.dir/build.make
bin/getCameraTransform: helpers/CMakeFiles/getCameraTransform.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/getCameraTransform"
	cd /home/nehchal/VisionProjectKrang/alvarCode/helpers && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/getCameraTransform.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
helpers/CMakeFiles/getCameraTransform.dir/build: bin/getCameraTransform
.PHONY : helpers/CMakeFiles/getCameraTransform.dir/build

helpers/CMakeFiles/getCameraTransform.dir/requires: helpers/CMakeFiles/getCameraTransform.dir/getCameraTransform.cpp.o.requires
.PHONY : helpers/CMakeFiles/getCameraTransform.dir/requires

helpers/CMakeFiles/getCameraTransform.dir/clean:
	cd /home/nehchal/VisionProjectKrang/alvarCode/helpers && $(CMAKE_COMMAND) -P CMakeFiles/getCameraTransform.dir/cmake_clean.cmake
.PHONY : helpers/CMakeFiles/getCameraTransform.dir/clean

helpers/CMakeFiles/getCameraTransform.dir/depend:
	cd /home/nehchal/VisionProjectKrang/alvarCode && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode/helpers /home/nehchal/VisionProjectKrang/alvarCode /home/nehchal/VisionProjectKrang/alvarCode/helpers /home/nehchal/VisionProjectKrang/alvarCode/helpers/CMakeFiles/getCameraTransform.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : helpers/CMakeFiles/getCameraTransform.dir/depend

