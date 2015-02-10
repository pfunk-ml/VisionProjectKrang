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
find_package( OpenCV 2.4.10 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


# Find Eigen
pkg_check_modules( EIGEN REQUIRED eigen3 )
  include_directories( ${EIGEN_INCLUDE_DIRS} )

# Find ACH
set( ACH_LIBS ach )

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

# Set this as include_directory
include_directories( ${CMAKE_SOURCE_DIR} )

# Add visualization libraries
add_subdirectory( viz )

# Add globalStuff library with global information about objects, cameras, and so
add_subdirectory( globalStuff )

# Add World Model structure
add_subdirectory( worldModel )

# Add helpers (to retrieve info we need)
add_subdirectory( helpers )

# Add cleanup 
add_executable( cleanup cleanup.cpp )
target_link_libraries( cleanup globalStuff ${ACH_LIBS} m)
set_target_properties( cleanup PROPERTIES RUNTIME_OUTPUT_DIRECTORY
${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# Add Marker detector process
# Returns markers detected per each camera (local transforms)
add_executable( camProcess camProcess.cpp )
target_link_libraries( camProcess globalStuff cvViewer glutViewer ${ACH_LIBS} ${alvar_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )

set_target_properties( camProcess PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )


# Add camera central process 
# Returns markers in global 
set( camCentralProcess_src CameraCentralProcess.cpp basicFilter.cpp )
set( camCentralProcess_hdr CameraCentralProcess.h basicFilter.h )

add_library( camCentralProcess ${camCentralProcess_src} ${camCentralProcess_hdr} )
target_link_libraries( camCentralProcess  globalStuff cvViewer glutViewer ${ACH_LIBS} m)
set_target_properties( camCentralProcess PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )


# camCentralProcessMain
add_executable( camCentralProcessMain camCentralProcessMain.cpp )
target_link_libraries( camCentralProcessMain camCentralProcess globalStuff worldModel ${ACH_LIBS} m)
set_target_properties( camCentralProcessMain PROPERTIES RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )



# Interface for Vision
add_subdirectory( testSimulation )


