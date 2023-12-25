find_package(Qt6 REQUIRED COMPONENTS Qml QuickControls2)

file(
  GLOB
  EASY_DEBUG_SOURCES
  "${CMAKE_SOURCE_DIR}/src/common/common/*.h"
  "${CMAKE_SOURCE_DIR}/src/common/common/*.cc"
  "${CMAKE_SOURCE_DIR}/src/toolbox/toolbox/*.h"
  "${CMAKE_SOURCE_DIR}/src/toolbox/toolbox/*.cc"
  "${CMAKE_SOURCE_DIR}/src/tools/tools/*.hh"
  "${CMAKE_SOURCE_DIR}/src/tools/tools/*.cc"
  "${CMAKE_SOURCE_DIR}/src/log/log/*.hh"
  "${CMAKE_SOURCE_DIR}/src/log/log/*.cc"
  "${CMAKE_SOURCE_DIR}/src/easydebug/*.h"
  "${CMAKE_SOURCE_DIR}/src/easydebug/*.cc"
  "${CMAKE_SOURCE_DIR}/qtswissarmyknifeqml.qrc"
  "${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc")

if(WIN32)
  list(APPEND EASY_DEBUG_SOURCES "${CMAKE_SOURCE_DIR}/windows.rc")
endif()

set(EASY_DEBUG_APP_SOURCES ${EASY_DEBUG_SOURCES})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/sak/EasyDebug")

if(SAK_IMPORT_MODULE_BLUETOOTH)
  message(STATUS "EasyDebug: Bluetooth is supportted")

  include_directories(${CMAKE_SOURCE_DIR}/src/optional/bluetooth)
  set(EASY_DEBUG_BLUETOOTH_SOURCES
      ${CMAKE_SOURCE_DIR}/src/optional/bluetooth/sakblecentraltool.h
      ${CMAKE_SOURCE_DIR}/src/optional/bluetooth/sakblecentraltool.cc
      ${CMAKE_SOURCE_DIR}/src/optional/bluetooth/sakblescanner.h
      ${CMAKE_SOURCE_DIR}/src/optional/bluetooth/sakblescanner.cc)
  list(APPEND EASY_DEBUG_APP_SOURCES ${EASY_DEBUG_BLUETOOTH_SOURCES})
endif()

sak_add_executable(EasyDebug ${EASY_DEBUG_APP_SOURCES})
sak_set_target_properties(EasyDebug)
sak_auto_execute_windeployqt(EasyDebug)

target_link_libraries(
  EasyDebug
  PRIVATE glog::glog
          Qt${QT_VERSION_MAJOR}::Core
          Qt${QT_VERSION_MAJOR}::Gui
          Qt${QT_VERSION_MAJOR}::Qml
          Qt${QT_VERSION_MAJOR}::SerialPort
          Qt${QT_VERSION_MAJOR}::QuickControls2
          Qt${QT_VERSION_MAJOR}::WebSockets
          Qt${QT_VERSION_MAJOR}::Widgets)
if(SAK_IMPORT_MODULE_BLUETOOTH)
  target_link_libraries(EasyDebug PRIVATE Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()
