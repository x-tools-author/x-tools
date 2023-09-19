find_package(Qt6 REQUIRED COMPONENTS Qml QuickControls2)

file(
  GLOB
  EASY_DEBUG_SOURCES
  "../common/common/*.h"
  "../common/common/*.cc"
  "../toolbox/toolbox/*.h"
  "../toolbox/toolbox/*.cc"
  "../tools/tools/*.hh"
  "../tools/tools/*.cc"
  "../log/log/*.hh"
  "../log/log/*.cc"
  "../guiapp/*.h"
  "../guiapp/*.cc"
  ${CMAKE_SOURCE_DIR}/easydebug.qrc
  ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)

set(EASY_DEBUG_APP_SOURCES ${EASY_DEBUG_SOURCES}
                           ${CMAKE_SOURCE_DIR/easydebug.qrc})
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

target_link_libraries(
  EasyDebug
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
          Qt${QT_VERSION_MAJOR}::Gui
          Qt${QT_VERSION_MAJOR}::Qml
          Qt${QT_VERSION_MAJOR}::SerialPort
          Qt${QT_VERSION_MAJOR}::QuickControls2
          Qt${QT_VERSION_MAJOR}::Bluetooth
          Qt${QT_VERSION_MAJOR}::WebSockets
          Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(
  CANBusStudio
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(CANBusStudio)
endif()
