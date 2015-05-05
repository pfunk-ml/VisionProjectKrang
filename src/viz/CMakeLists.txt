#####################################
# Create visualization helpers      #
#####################################

# cvViewer utility
add_library( cvViewer STATIC CvTestbed.h
    CvTestbed.cpp Shared.h )
set_target_properties( cvViewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

# SharedGlutViewer
add_library( glutViewer STATIC
 GlutViewer.h GlutViewer.cpp )

target_link_libraries(glutViewer ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )

set_target_properties( glutViewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY 
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
