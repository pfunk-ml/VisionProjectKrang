cmake_minimum_required( VERSION 2.8 )

# Find OpenGL
find_package( OpenGL REQUIRED)
if( OPENGL_FOUND )
  include_directories( ${OPENGL_INCLUDE_DIRS} )
endif()

# Find GLUT
find_package( GLUT REQUIRED )
if( GLUT_FOUND )
  include_directories( ${GLUT_INCLUDE_DIRS} )
endif()

# PkgConfig
find_package( PkgConfig REQUIRED)

# Find OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Find ALVAR
pkg_check_modules( alvar REQUIRED ALVAR )
if( alvar_FOUND )
  include_directories( ${alvar_INCLUDE_DIRS} )
  link_directories( ${alvar_LIBRARY_DIRS} )
  message( "ALVAR INCLUDE DIRS: ${alvar_INCLUDE_DIRS}" )
  message( "ALVAR LIBRARY DIRS: ${alvar_LIBRARY_DIRS}" )
  message( "ALVAR LIBRARIES: ${alvar_LIBRARIES}" )
  message( STATUS "ALVAR FOUND!")
endif()

# Add 2 utility libraries

# SharedSamples
add_library(SharedSamples STATIC CvTestbed.h
    CvTestbed.cpp Shared.h )
set_target_properties( SharedSamples PROPERTIES LIBRARY_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

#SharedGlutViewer
add_library(SharedGlutViewer STATIC
 GlutViewer.h GlutViewer.cpp )

target_link_libraries(SharedGlutViewer ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )

set_target_properties( SharedGlutViewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

# Add camera calibration file
add_executable( runCalibration runCalibration.cpp )
target_link_libraries( runCalibration SharedSamples SharedGlutViewer ${alvar_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )

set_target_properties( runCalibration PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# Add marker generator file
add_executable( generateMarkers generateMarkers.cpp )
target_link_libraries( generateMarkers SharedSamples SharedGlutViewer ${alvar_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )

set_target_properties( generateMarkers PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )



# Add sample marker detector
add_executable( sampleMarkerDetector markerDetector.cpp )
target_link_libraries( sampleMarkerDetector SharedSamples SharedGlutViewer ${alvar_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )

set_target_properties( sampleMarkerDetector PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

