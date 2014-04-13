
######################
# HELPERS            #
######################

# Executable to generate Markers
add_executable( generateMarkers generateMarkers.cpp )
target_link_libraries( generateMarkers cvViewer glutViewer ${alvar_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )

set_target_properties( generateMarkers PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# Executable to generate camera transform w.r.t. one marker
add_executable( getCameraTransform getCameraTransform.cpp )
target_link_libraries( getCameraTransform globalStuff cvViewer glutViewer ${alvar_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )

set_target_properties( getCameraTransform PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# Generate .xml calibration file
add_executable( runCalibration runCalibration.cpp )
target_link_libraries( runCalibration cvViewer glutViewer ${alvar_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )

set_target_properties( runCalibration PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )


