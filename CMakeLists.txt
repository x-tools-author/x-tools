cmake_minimum_required(VERSION 3.16)

project(
  QtSwissArmyKnife
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
  QT NAMES Qt6 Qt5 REQUIRED
  COMPONENTS Core
             Gui
             Widgets
             WebSockets
             Network
             SerialPort
             SerialBus
             Bluetooth)
find_package(
  Qt${QT_VERSION_MAJOR} REQUIRED
  COMPONENTS Core
             Gui
             Widgets
             WebSockets
             Network
             SerialPort
             SerialBus
             Bluetooth)

if(${QT_VERSION} VERSION_LESS "6.5")
  message(WARNING "The Qt version is too old, please using Qt6.5 or later.")
endif()

# CRCAssistant
option(SAK_CRCASSISTANT "Enable CRCAssistant for QtSwissArmyKnife" ON)
if(SAK_CRCASSISTANT)
  include_directories(src/assistants/crcassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_CRCASSISTANT)
endif()

# FileCheckAssistant
option(SAK_FILECHECKASSISTANT "" ON)
if(SAK_FILECHECKASSISTANT)
  include_directories(src/assistants/filecheckassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_FILECHECKASSISTANT)
endif()

# AsciiAssistant
option(SAK_ASCIIASSISTANT "" ON)
if(SAK_ASCIIASSISTANT)
  include_directories(src/assistants/asciiassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_ASCIIASSISTANT)
endif()

# BroadcastAssistant
option(SAK_BROCASTASSISTANT "" ON)
if(SAK_BROCASTASSISTANT)
  include_directories(src/assistants/broadcastassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_BROADCASTASSISTANT)
endif()

# NumberAssistant
option(SAK_NUMBERASSISTANT "" ON)
if(SAK_NUMBERASSISTANT)
  include_directories(src/assistants/floatassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_FLOATASSISTANT)
endif()

# StringAssistant
option(SAK_STRINGASSISTANT "" ON)
if(SAK_STRINGASSISTANT)
  include_directories(src/assistants/stringassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_STRINGASSISTANT)
endif()

# Assistant module
include_directories(src/assistants)
include_directories(src/assistantsui)
file(GLOB_RECURSE ASSISTANT_SOURCES "src/assistants/*h" "src/assistants/*.cc")
file(GLOB_RECURSE ASSISTANT_UI_SOURCES "src/assistantsui/*h"
     "src/assistantsui/*.cc" "src/assistantsui/*.ui")

# CAN bus module
include_directories(src/canbus/canbus)
include_directories(src/canbus/canbusui)
file(GLOB_RECURSE CANBUS_SOURCES "src/canbus/canbus/*h"
     "src/canbus/canbus/*.cc")
file(GLOB_RECURSE CANBUS_UI_SOURCES "src/canbus/canbusui/*h"
     "src/canbus/canbusui/*.cc" "src/canbus/canbusui/*.ui")

# Commom module
include_directories(src/common)
include_directories(src/commonui)
file(GLOB_RECURSE COMMON_SOURCES "src/common/*h" "src/common/*.cc")
file(GLOB_RECURSE COMMON_UI_SOURCES "src/commonui/*h" "src/commonui/*.cc"
     "src/commonui/*.ui")

# Log module
include_directories(src/log)
include_directories(src/logui)
file(GLOB_RECURSE LOG_SOURCES "src/log/*h" "src/log/*.cc")
file(GLOB_RECURSE LOG_UI_SOURCES "src/logui/*h" "src/logui/*.cc"
     "src/logui/*.ui")


# Modbus module
option(SAK_MODBUS "" ON)
if(SAK_MODBUS)
include_directories(src/modbus/modbus)
include_directories(src/modbus/modbusui)
file(GLOB_RECURSE MODBUS_SOURCES "src/modbus/modbus/*h"
     "src/modbus/modbus/*.cc")
file(GLOB_RECURSE MODBUS_UI_SOURCES "src/modbus/modbusui/*h"
     "src/modbus/modbusui/*.cc" "src/modbus/modbusui/*.ui")
add_definitions(-DSAK_IMPORT_MODULE_MODBUS)
endif()

# ToolBox module
include_directories(src/toolbox)
include_directories(src/toolboxui)
file(GLOB_RECURSE TOOLBOX_SOURCES "src/toolbox/*h" "src/toolbox/*.cc")
file(GLOB_RECURSE TOOLBOX_UI_SOURCES "src/toolboxui/*h" "src/toolboxui/*.cc"
     "src/toolboxui/*.ui")

# Tools module
include_directories(src/tools)
include_directories(src/toolsui)
file(GLOB_RECURSE TOOLS_SOURCES "src/tools/*h" "src/tools/*.cc")
file(GLOB_RECURSE TOOLS_UI_SOURCES "src/toolsui/*h" "src/toolsui/*.cc"
     "src/toolsui/*.ui")

# App(QtSwissArmyKnife) Tools module
include_directories(src)
file(GLOB APP_SOURCES "src/*.h" "src/*.cc")

add_definitions(
  -DSAK_HOST_ADDRESS_ANY="Any"
  -DSAK_CLEAR_MESSAGE_INTERVAL=8000
  -DSAK_STYLE_DEFAULT="Fusion"
  -DSAK_EDITION="beta1"
  -DSAK_VERSION="5.0.0"
  -DSAK_AUTHOR="Qsaker"
  -DSAK_AUTHOR_EMAIL="qsaker@foxmail.com"
  -DSAK_GITHUB_REPOSITORY_URL="https://github.com/qsaker/QtSwissArmyKnife"
  -DSAK_GITEE_REPOSITORY_URL="https://gitee.com/qsaker/QtSwissArmyKnife"
  -DSAK_IMPORT_MODULE_CANBUSUI)

set(SAK_APP_SOURCES
    ${ASSISTANT_SOURCES}
    ${ASSISTANT_UI_SOURCES}
    ${CANBUS_SOURCES}
    ${CANBUS_UI_SOURCES}
    ${COMMON_SOURCES}
    ${COMMON_UI_SOURCES}
    ${LOG_SOURCES}
    ${LOG_UI_SOURCES}
    ${MODBUS_SOURCES}
    ${MODBUS_UI_SOURCES}
    ${TOOLBOX_SOURCES}
    ${TOOLBOX_UI_SOURCES}
    ${TOOLS_SOURCES}
    ${TOOLS_UI_SOURCES}
    ${APP_SOURCES}
    SAKResources.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(QtSwissArmyKnife MANUAL_FINALIZATION ${SAK_APP_SOURCES})
else()
  if(ANDROID)
    add_library(QtSwissArmyKnife SHARED ${SAK_APP_SOURCES})
  else()
    add_executable(QtSwissArmyKnife ${SAK_APP_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(
  QtSwissArmyKnife
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets
          Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::SerialPort
          Qt${QT_VERSION_MAJOR}::SerialBus Qt${QT_VERSION_MAJOR}::Bluetooth)

set_target_properties(
  QtSwissArmyKnife
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)
install(
  TARGETS QtSwissArmyKnife
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(QtSwissArmyKnife)
endif()

execute_process(
  COMMAND git log -1 --pretty=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Last commit: ${GIT_COMMIT}")

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(WIN_DEPLOY_QT "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/windeployqt.exe")
  message(STATUS ${Qt${QT_VERSION_MAJOR}_WIN_DEPLOY_QT})
  cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH compiler_path)
  add_custom_command(
    TARGET QtSwissArmyKnife
    POST_BUILD
    COMMAND ${WIN_DEPLOY_QT} "${CMAKE_BINARY_DIR}/QtSwissArmyKnife.exe")
endif()
