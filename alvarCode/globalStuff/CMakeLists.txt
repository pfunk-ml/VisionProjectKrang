# #################### #
# Global information   #
# #################### #
add_library( globalStuff globalData.cpp globalTransforms.cpp )
target_link_libraries( globalStuff )
set_target_properties( globalStuff PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

