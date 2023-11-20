cmake_minimum_required(VERSION 3.16)

project(
  QtSwissArmyKnife
  VERSION 1.0
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

if(${QT_VERSION} VERSION_LESS "6.4")
  message(WARNING "The Qt version is too old, please using Qt6.4 or later.")
endif()

# ##############################################################################
# Common definitioin
add_definitions(
  -DSAK_HOST_ADDRESS_ANY="Any"
  -DSAK_CLEAR_MESSAGE_INTERVAL=8000
  -DSAK_STYLE_DEFAULT="Fusion"
  -DSAK_EDITION="beta1"
  -DSAK_VERSION="5.1.0"
  -DSAK_AUTHOR="Qsaker"
  -DSAK_AUTHOR_EMAIL="qsaker@foxmail.com"
  -DSAK_GITHUB_REPOSITORY_URL="https://github.com/qsaker/QtSwissArmyKnife"
  -DSAK_GITEE_REPOSITORY_URL="https://gitee.com/qsaker/QtSwissArmyKnife")
# ##############################################################################

# ##############################################################################
# Git information
execute_process(
  COMMAND git log -1 --pretty=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Last commit: ${GIT_COMMIT}")
add_compile_definitions(SAK_GIT_COMMIT="${GIT_COMMIT}")
execute_process(
  COMMAND git log -1 --format=%ci
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE LAST_COMMIT_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Last commit time: ${LAST_COMMIT_DATE}")
add_compile_definitions(SAK_COMMIT_DATE="${LAST_COMMIT_DATE}")
# ##############################################################################

# ##############################################################################
# Assistant module
include_directories(src/assistants)
include_directories(src/assistantsui)
list(APPEND SAK_ASSISTANT_SOURCES src/assistants/sakassistantsfactory.h
     src/assistants/sakassistantsfactory.cc)

macro(sak_import_assistant assistant_dir_name)
  string(TOUPPER ${assistant_dir_name} upper_name)
  option(SAK_IMPORT_MODULE_${upper_name} "" ON)
  if(SAK_IMPORT_MODULE_${upper_name})
    include_directories(
      ${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src)
    add_compile_definitions(SAK_IMPORT_MODULE_${upper_name})
    file(
      GLOB_RECURSE
      ASSISTANT_SOURCE
      "${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src/*h"
      "${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src/*.cc"
      "${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src/*.ui")
    list(APPEND SAK_ASSISTANT_SOURCES ${ASSISTANT_SOURCE})
  endif()
endmacro()

sak_import_assistant("asciiassistant")
sak_import_assistant("base64assistant")
sak_import_assistant("broadcastassistant")
sak_import_assistant("crcassistant")
sak_import_assistant("filecheckassistant")
sak_import_assistant("numberassistant")
sak_import_assistant("stringassistant")
# ##############################################################################

# ##############################################################################
# QtSwissArmyKnife src files
macro(sak_add_src src_dir_name)
  set(SRC_PRE ${CMAKE_SOURCE_DIR}/src/${src_dir_name})
  include_directories(${SRC_PRE}/${src_dir_name})
  include_directories(${SRC_PRE}/${src_dir_name}ui)
  file(
    GLOB_RECURSE
    SRC_SOURCE
    "${SRC_PRE}/${src_dir_name}/*h"
    "${SRC_PRE}/${src_dir_name}/*.cc"
    "${SRC_PRE}/${src_dir_name}ui/*.h"
    "${SRC_PRE}/${src_dir_name}ui/*.cc"
    "${SRC_PRE}/${src_dir_name}ui/*.ui")
  list(APPEND SAK_SRC_SOURCES ${SRC_SOURCE})
endmacro()

sak_add_src("common")
sak_add_src("log")
sak_add_src("toolbox")
sak_add_src("tools")
# ##############################################################################

# ##############################################################################
# QtSwissArmyKnife module files
macro(sak_import_module src_dir_name)
  string(TOUPPER ${src_dir_name} upper_dir_name)
  option(SAK_IMPORT_MODULE_${upper_dir_name} "" ON)
  if(SAK_IMPORT_MODULE_${upper_dir_name})
    set(SRC_PRE ${CMAKE_SOURCE_DIR}/src/${src_dir_name})
    include_directories(${SRC_PRE}/${src_dir_name})
    include_directories(${SRC_PRE}/${src_dir_name}ui)
    add_compile_definitions(SAK_IMPORT_MODULE_${upper_dir_name})
    file(
      GLOB_RECURSE
      SRC_SOURCE
      "${SRC_PRE}/${src_dir_name}/*h"
      "${SRC_PRE}/${src_dir_name}/*.cc"
      "${SRC_PRE}/${src_dir_name}ui/*.h"
      "${SRC_PRE}/${src_dir_name}ui/*.cc"
      "${SRC_PRE}/${src_dir_name}ui/*.ui")
    list(APPEND SAK_SRC_SOURCES ${SRC_SOURCE})
  endif()
endmacro()

sak_import_module("canbusstudio")
sak_import_module("modbusstudio")
# ##############################################################################

# ##############################################################################
# QtSwissArmyKnife)
include_directories(src)
file(GLOB APP_SOURCES "src/*.h" "src/*.cc" "src/*.ui" ".cmake/*.cmake")

option(SAK_BUILD_FOR_APP_SOTRE "Build for Microsoft/Apple app store." OFF)
if(SAK_BUILD_FOR_APP_SOTRE AND CMAKE_BUILD_TYPE)
  add_definitions(-DSAK_RELEASE_FOR_APP_STORE)
  set(SAK_IMPORT_MODULE_CANBUSSTUDIO OFF)
endif()

set(SAK_APP_SOURCES ${SAK_ASSISTANT_SOURCES} ${SAK_SRC_SOURCES} ${APP_SOURCES}
                    qtswissarmyknife.qrc windows.rc)

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
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
install(
  TARGETS QtSwissArmyKnife
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(QtSwissArmyKnife)
endif()
# ##############################################################################

# ##############################################################################
# Custom build target.
option(APP_EABLE_ASSISTANTS "Enable to build assistants." ON)
if(APP_EABLE_ASSISTANTS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/assistants)
endif()

option(APP_EABLE_CANBUSSTUDIO "Enable to build CANBusStudio." ON)
if(APP_EABLE_CANBUSSTUDIO)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/canbusstudio)
endif()

option(APP_EABLE_MODBUSSTUDIO "Enable to build ModbusStudio." ON)
if(APP_EABLE_MODBUSSTUDIO)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/modbusstudio)
endif()

# For Qt6.4 and later.
if(QT_VERSION_MAJOR EQUAL 6 AND QT_VERSION_MINOR GREATER_EQUAL 4)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/easydebug)
endif()

include(${CMAKE_SOURCE_DIR}/.cmake/deploy.cmake)
sak_add_deploy()
include(${CMAKE_SOURCE_DIR}/.cmake/internationalization.cmake)
sak_add_lupdate()
sak_add_lrelease()

# The private module is not open source.
include(${CMAKE_SOURCE_DIR}/Private/Private.cmake)
