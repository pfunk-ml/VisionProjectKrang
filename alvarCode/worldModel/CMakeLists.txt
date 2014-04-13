# World Model
set( WorldModel_src ARCamera.cpp ARMarker.cpp WorldModel.cpp )
set( WorldModel_hdr ARCamera.h ARMarker.h WorldModel.h )

add_library( worldModel ${WorldModel_src} ${WorldModel_hdr} )
target_link_libraries( worldModel )
set_target_properties( worldModel PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
