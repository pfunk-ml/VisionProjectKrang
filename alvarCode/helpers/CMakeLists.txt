
######################
# HELPERS            #
######################
#set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}" )
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}" )

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

# Test final perception output channel
add_executable( testSubscriber testSubscriber.cpp )
target_link_libraries( testSubscriber globalStuff ${ACH_LIBS} )

set_target_properties( testSubscriber PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )


# Test filter
# Use dummy created data
add_executable( testFilter testFilter.cpp ../basicFilter.cpp )
target_link_libraries( testFilter ${ACH_LIBS} )

set_target_properties( testFilter PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )



# Test filter plot
# Use actual data
add_executable( testFilterPlot testFilterPlot.cpp ../basicFilter.cpp )
target_link_libraries( testFilterPlot globalStuff ${ACH_LIBS} )

set_target_properties( testFilterPlot PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )


