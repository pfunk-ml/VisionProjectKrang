# #################### #
# Global information   #
# #################### #
cmake_minimum_required( VERSION 2.8 )

# PkgConfig
find_package(PkgConfig REQUIRED)

# Find Eigen
pkg_check_modules( EIGEN REQUIRED eigen3 )
include_directories( ${EIGEN_INCLUDE_DIRS} )

# Find Jsoncpp
find_library(jsoncpp NAMES jsoncpp HINTS jsoncpp/lib)

# Executable to test Json stuff
add_executable( testGlobals globalTest.cpp )
target_link_libraries( testGlobals globalStuff jsoncpp)

add_library( globalStuff globalData.cpp globalTransforms.cpp optparser.cpp Object.cpp)
target_link_libraries( globalStuff jsoncpp worldModel)
set_target_properties( globalStuff PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
