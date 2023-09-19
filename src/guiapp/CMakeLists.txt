file(
  GLOB
  EASY_DEBUG_SOURCES
  "src/common/common/*.h"
  "src/common/common/*.cc"
  "src/toolbox/toolbox/*.h"
  "src/toolbox/toolbox/*.cc"
  "src/tools/tools/*.hh"
  "src/tools/tools/*.cc"
  "src/guiapp/*.h"
  "src/guiapp/*.cc")

set(EASY_DEBUG_APP_SOURCES ${EASY_DEBUG_SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/EasyDebug")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(EasyDebug MANUAL_FINALIZATION ${EASY_DEBUG_APP_SOURCES})
else()
  if(ANDROID)
    add_library(EasyDebug SHARED ${EASY_DEBUG_APP_SOURCES})
  else()
    add_executable(EasyDebug ${EASY_DEBUG_APP_SOURCES})
  endif(ANDROID)
endif()
