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

# Find YAML
pkg_check_modules( YAML_CPP REQUIRED yaml-cpp )
if( YAML_CPP_FOUND )
  message( "YAML-CPP FOUND YEAH!")
    include_directories( ${YAML_CPP_INCLUDE_DIRS} )
    link_directories( ${YAML_CPP_LIBRARY_DIRS} )    
else()
  message("YAML-CPP NOT FOUND ARGH!")
endif()



# cam Main process (run for each camera)
add_executable( camProcess camProcessMain.cpp CameraProcess.cpp CameraProcess.h Object.h globalData.h  )
target_link_libraries( camProcess ${ARToolkit_LIBRARIES} 
  ${Gst_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${ACH_LIBS} ${YAML_CPP_LIBRARIES} m)
set_target_properties( camProcess PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )



# Integrated Perception Module
set( PerceptionModel_src CameraCentralProcess.cpp WorldModel.cpp ARCamera.cpp ARMarker.cpp globalTransforms.cpp )
set( PerceptionModel_hdr CameraCentralProcess.h WorldModel.h ARCamera.h ARMarker.h globalTransforms.h )

add_library( PerceptionModule ${PerceptionModel_src} ${PerceptionModel_hdr} )
target_link_libraries( PerceptionModule ${ACH_LIBS} m)
set_target_properties( PerceptionModule PROPERTIES LIBRARY_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )


# camCentralProcessMain
add_executable( camCentralProcessMain 
  camCentralProcessMain.cpp )
target_link_libraries( camCentralProcessMain PerceptionModule ${ACH_LIBS} m)
set_target_properties( camCentralProcessMain PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )


# Interface for Vision
add_subdirectory( cameraController )
