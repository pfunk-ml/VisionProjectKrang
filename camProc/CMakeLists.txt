cmake_minimum_required( VERSION 2.8 )

set( CMAKE_CXX_FLAGS "-lm" )
set(CMAKE_C_FLAGS "-lm" )
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

# Find GST
pkg_check_modules( Gst REQUIRED gstreamer-0.10 )
if( Gst_FOUND )
  include_directories( ${Gst_INCLUDE_DIRS} )
endif()

# Find AR
pkg_check_modules( ARToolkit REQUIRED AR )
if( ARToolkit_FOUND )
  message( "AR FOUND YEAH!")
    include_directories( ${ARToolkit_INCLUDE_DIRS} )
    link_directories( ${ARToolkit_LIBRARY_DIRS} )
else()
  message("AR NOT FOUND ARGH!")
endif()

# Find Eigen
pkg_check_modules( EIGEN REQUIRED eigen3 )
  include_directories( ${EIGEN_INCLUDE_DIRS} )

# Find ACH
set( ACH_LIBS ach )


# camCentralProcess  - multiple markers
add_executable( camProcess camProcessMain.cpp CameraProcess.h Object.h globalData.h  )
target_link_libraries( camProcess ${ARToolkit_LIBRARIES} 
  ${Gst_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${ACH_LIBS} m)
set_target_properties( camProcess PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )


# Interface for Vision
add_subdirectory( cameraController )
