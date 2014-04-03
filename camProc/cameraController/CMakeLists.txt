# Project
cmake_minimum_required( VERSION 2.8 )
project ( tabTest )

message (STATUS "Creating Makefile for manip")


set(CMAKE_INSTALL_PREFIX /usr/local )
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -std=c++0x")

# DART
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_INSTALL_PREFIX}/share/dart")
#find_package(DARTExt REQUIRED)
find_package(DART REQUIRED CONFIG)

#include_directories(${DARTExt_INCLUDE_DIRS})
include_directories(${DART_INCLUDE_DIRS})
link_directories(${DART_LIBRARY_DIRS})

# GRIP
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_INSTALL_PREFIX}/share/grip")
find_package(GRIP REQUIRED)
include_directories(${GRIP_INCLUDE_DIRS})
link_directories(${GRIP_LIBRARY_DIRS})


# Search all header and source files
set ( cameraController_src cameraController.cpp 
  cameraControllerApp.cpp )

set ( cameraController_hdr cameraController.h )

# wxWidgets
set (wxWidgets_USE_LIBS base core gl)

find_package (wxWidgets)
if (wxWidgets_FOUND)
  include (${wxWidgets_USE_FILE})
  include_directories(${WXWIDGETS_SHARED_INCLUDE_DIRS})
  link_directories(${WXWIDGETS_SHARED_LIBRARY_DIRS})
  
  add_executable ( cameraController ${cameraController_src} ${cameraController_hdr})
  
  target_link_libraries ( cameraController ${GRIP_LIBRARIES} ${DART_LIBRARIES} ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES}  ${ACH_LIBS} ) # ${DARTExt_LIBRARIES}
  set_target_properties( cameraController PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

else (wxWidgets_FOUND)
  message ("wxWidgets not found!")
endif (wxWidgets_FOUND)


