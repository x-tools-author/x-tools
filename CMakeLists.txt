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

set(SAK_QT_COMPONENTS
    Core
    Gui
    Widgets
    WebSockets
    Network
    SerialPort
    SerialBus
    Bluetooth)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${SAK_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${SAK_QT_COMPONENTS})

include(.cmake/sak_common.cmake)
include(.cmake/sak_common_deploy.cmake)

sak_get_last_commit(${CMAKE_CURRENT_SOURCE_DIR} "SAK")
sak_get_last_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "SAK")

# Assistant module
include_directories(src/assistants)
include_directories(src/assistantsui)
list(APPEND SAK_ASSISTANT_SOURCES src/assistants/sakassistantsfactory.h
     src/assistants/sakassistantsfactory.cc)

macro(sak_import_assistant assistant_dir_name)
  string(TOUPPER ${assistant_dir_name} upper_name)
  option(SAK_IMPORT_MODULE_${upper_name} "" ON)
  if(SAK_IMPORT_MODULE_${upper_name})
    include_directories(${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src)
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

if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
  sak_import_module("canbusstudio")
  sak_import_module("modbusstudio")
endif()

# QtSwissArmyKnife)
include_directories(src)
file(GLOB APP_SOURCES "src/*.h" "src/*.cc" "src/*.ui" ".cmake/*.cmake")

option(SAK_BUILD_FOR_APP_SOTRE "Build for Microsoft/Apple app store." OFF)
if(SAK_BUILD_FOR_APP_SOTRE AND CMAKE_BUILD_TYPE)
  add_definitions(-DSAK_RELEASE_FOR_APP_STORE)
  set(SAK_IMPORT_MODULE_CANBUSSTUDIO OFF)
endif()

list(APPEND SAK_APP_SOURCES ${SAK_ASSISTANT_SOURCES} ${SAK_SRC_SOURCES} ${APP_SOURCES})
list(APPEND SAK_APP_SOURCES qtswissarmyknife.qrc windows.rc)
sak_add_executable("QtSwissArmyKnife" ${SAK_APP_SOURCES})
sak_auto_execute_deployqt(QtSwissArmyKnife)
sak_set_target_properties(QtSwissArmyKnife)
target_link_libraries(
  QtSwissArmyKnife
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets
          Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::SerialPort
          Qt${QT_VERSION_MAJOR}::SerialBus Qt${QT_VERSION_MAJOR}::Bluetooth)
install(
  TARGETS QtSwissArmyKnife
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Custom build target.
option(APP_EABLE_ASSISTANTS "Enable to build assistants." ON)
if(APP_EABLE_ASSISTANTS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/assistants)
endif()

if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
option(APP_EABLE_CANBUSSTUDIO "Enable to build CANBusStudio." ON)
if(APP_EABLE_CANBUSSTUDIO)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/canbusstudio)
endif()
option(APP_EABLE_MODBUSSTUDIO "Enable to build ModbusStudio." ON)
if(APP_EABLE_MODBUSSTUDIO)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/modbusstudio)
endif()
endif()

# For Qt6.4 and later.
if(QT_VERSION_MAJOR EQUAL 6 AND QT_VERSION_MINOR GREATER_EQUAL 4)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/easydebug)
endif()

include(${CMAKE_SOURCE_DIR}/.cmake/internationalization.cmake)
sak_add_lupdate()
sak_add_lrelease()

# The private module is not open source.
include(${CMAKE_SOURCE_DIR}/Private/Private.cmake)
