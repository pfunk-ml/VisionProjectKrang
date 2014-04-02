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

# Find GST
pkg_check_modules( Gst REQUIRED gstreamer-0.10 )
if( Gst_FOUND )
  include_directories( ${Gst_INCLUDE_DIRS} )
endif()

# Find AR
pkg_check_modules( ARToolkit REQUIRED AR )
if( AR_FOUND )
    include_directories( ${ARToolkit_INCLUDE_DIRS} )
    link_directories( ${ARToolkit_LIBRARY_DIRS} )
endif()

# Find Eigen
pkg_check_modules( EIGEN REQUIRED eigen3 )
  include_directories( ${EIGEN_INCLUDE_DIRS} )



# Find ACH
set( ACH_LIBS ach )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# Create binary (put libraries in order!)
add_executable( twoViews twoView.c )
target_link_libraries( twoViews ${ARToolkit_LIBRARIES} 
  ${Gst_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )
set_target_properties( twoViews PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

add_executable( oneView oneView.cpp )
target_link_libraries( oneView ${ARToolkit_LIBRARIES} 
  ${Gst_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )
set_target_properties( oneView PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

add_executable( achTest achtest.c ) # achTest.cpp
target_link_libraries( achTest ${ACH_LIBS} ${ARToolkit_LIBRARIES} 
  ${Gst_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )
set_target_properties( achTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# cameraProcessMain
add_executable( camProcessMain camProcessMain.cpp CameraProcess.cpp CameraProcess.h )
target_link_libraries( camProcessMain ${ARToolkit_LIBRARIES} 
  ${Gst_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )
set_target_properties( camProcessMain PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
