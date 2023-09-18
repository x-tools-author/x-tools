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
  file(GLOB_RECURSE SAK_CRCASSISTANT_SOURCE
       "src/assistants/crcassistant/src/*h"
       "src/assistants/crcassistant/src/*.cc")
endif()

# Base64Assistant
option(SAK_BASE64ASSISTANT "" ON)
if(SAK_BASE64ASSISTANT)
  include_directories(src/assistants/base64assistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_BASE64ASSISTANT)
  file(GLOB_RECURSE SAK_BASE64ASSISTANT_SOURCE
       "src/assistants/base64assistant/src/*h"
       "src/assistants/base64assistant/src/*.cc")
endif()

# FileCheckAssistant
option(SAK_FILECHECKASSISTANT "" ON)
if(SAK_FILECHECKASSISTANT)
  include_directories(src/assistants/filecheckassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_FILECHECKASSISTANT)
  file(GLOB_RECURSE SAK_FILECHECKASSISTANT_SOURCE
       "src/assistants/filecheckassistant/src/*h"
       "src/assistants/filecheckassistant/src/*.cc")
endif()

# AsciiAssistant
option(SAK_ASCIIASSISTANT "" ON)
if(SAK_ASCIIASSISTANT)
  include_directories(src/assistants/asciiassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_ASCIIASSISTANT)
  file(GLOB_RECURSE SAK_ASCIIASSISTANT_SOURCE
       "src/assistants/asciiassistant/src/*h"
       "src/assistants/asciiassistant/src/*.cc")
endif()

# BroadcastAssistant
option(SAK_BROCASTASSISTANT "" ON)
if(SAK_BROCASTASSISTANT)
  include_directories(src/assistants/broadcastassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_BROADCASTASSISTANT)
  file(GLOB_RECURSE SAK_BROCASTASSISTANT_SOURCE
       "src/assistants/broadcastassistant/src/*h"
       "src/assistants/broadcastassistant/src/*.cc")
endif()

# NumberAssistant
option(SAK_NUMBERASSISTANT "" ON)
if(SAK_NUMBERASSISTANT)
  include_directories(src/assistants/numberassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_FLOATASSISTANT)
  file(GLOB_RECURSE SAK_NUMBERASSISTANT_SOURCE
       "src/assistants/numberassistant/src/*h"
       "src/assistants/numberassistant/src/*.cc")
endif()

# StringAssistant
option(SAK_STRINGASSISTANT "" ON)
if(SAK_STRINGASSISTANT)
  include_directories(src/assistants/stringassistant/src)
  add_definitions(-DSAK_IMPORT_MODULE_STRINGASSISTANT)
  file(GLOB_RECURSE SAK_STRINGASSISTANT_SOURCE
       "src/assistants/stringassistant/src/*h"
       "src/assistants/stringassistant/src/*.cc")
endif()

# Assistant module
include_directories(src/assistants)
include_directories(src/assistantsui)
set(ASSISTANT_SOURCES
    ${SAK_ASCIIASSISTANT_SOURCE}
    ${SAK_BASE64ASSISTANT_SOURCE}
    ${SAK_BROCASTASSISTANT_SOURCE}
    ${SAK_CRCASSISTANT_SOURCE}
    ${SAK_FILECHECKASSISTANT_SOURCE}
    ${SAK_NUMBERASSISTANT_SOURCE}
    ${SAK_STRINGASSISTANT_SOURCE}
    src/assistants/sakassistantsfactory.h
    src/assistants/sakassistantsfactory.cc)
file(GLOB_RECURSE ASSISTANT_UI_SOURCES "src/assistantsui/*h"
     "src/assistantsui/*.cc" "src/assistantsui/*.ui")

# CAN bus module
option(SAK_CANBUS "" ON)
if(SAK_CANBUS)
  include_directories(src/canbus/canbus)
  include_directories(src/canbus/canbusui)
  file(GLOB_RECURSE CANBUS_SOURCES "src/canbus/canbus/*h"
       "src/canbus/canbus/*.cc")
  file(GLOB_RECURSE CANBUS_UI_SOURCES "src/canbus/canbusui/*h"
       "src/canbus/canbusui/*.cc" "src/canbus/canbusui/*.ui")
  add_definitions(-DSAK_IMPORT_MODULE_CANBUSUI)
endif()

# Commom module
include_directories(src/common/common)
include_directories(src/common/commonui)
file(GLOB_RECURSE COMMON_SOURCES "src/common/common/*h"
     "src/common/common/*.cc")
file(GLOB_RECURSE COMMON_UI_SOURCES "src/common/commonui/*h"
     "src/common/commonui/*.cc" "src/common/commonui/*.ui")

# Log module
include_directories(src/log/log)
include_directories(src/log/logui)
file(GLOB_RECURSE LOG_SOURCES "src/log/log/*h" "src/log/log/*.cc")
file(GLOB_RECURSE LOG_UI_SOURCES "src/log/logui/*h" "src/log/logui/*.cc"
     "src/log/logui/*.ui")

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
  -DSAK_GITEE_REPOSITORY_URL="https://gitee.com/qsaker/QtSwissArmyKnife")

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
    SAKResources.qrc
    windows.rc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
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
  PROPERTIES MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE
             ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
# cmake-format on

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

option(SAK_AUTO_DEPLOY "" OFF)
if(CMAKE_SYSTEM_NAME MATCHES "Windows" AND SAK_AUTO_DEPLOY)
  set(WIN_DEPLOY_QT "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/windeployqt.exe")
  message(STATUS ${Qt${QT_VERSION_MAJOR}_WIN_DEPLOY_QT})
  cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH compiler_path)
  add_custom_command(
    TARGET QtSwissArmyKnife
    POST_BUILD
    COMMAND ${WIN_DEPLOY_QT}
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QtSwissArmyKnife.exe")
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" AND MSVC)
    add_custom_command(
      TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        "${compiler_path}/VCRUNTIME140.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        "${compiler_path}/VCRUNTIME140_1.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${compiler_path}/MSVCP140.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/MSVCP140_1.dll"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/MSVCP140_2.dll"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endif()
endif()

add_subdirectory(src)
