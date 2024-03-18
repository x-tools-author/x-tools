cmake_minimum_required(VERSION 3.21)

project(
  QtSwissArmyKnife
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SAK_QT_COMPONENTS
    Core
    Gui
    Widgets
    WebSockets
    Network
    SerialPort
    SerialBus)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${SAK_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${SAK_QT_COMPONENTS})
add_compile_definitions(SAK_BUILD_WITH_SAK)

if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
  find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Bluetooth)
  if(Qt${QT_VERSION_MAJOR}Bluetooth_FOUND)
    message(STATUS "Qt${QT_VERSION_MAJOR}::Bluetooth found.")
    option(SAK_IMPORT_MODULE_BLUETOOTH "Import Bluetooth module." ON)
    add_compile_definitions(SAK_IMPORT_MODULE_BLUETOOTH)
  else()
    message(STATUS "Qt${QT_VERSION_MAJOR}::Bluetooth not found.")
  endif()
endif()

set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF)

include(.cmake/sak_common.cmake)
include(.cmake/sak_git.cmake)
include(.cmake/sak_environment.cmake)
include(.cmake/sak_deploy.cmake)
include(.cmake/sak_font_fontawesome.cmake)
include(.cmake/sak_installer.cmake)

include(.cmake/sak_3rd_glog.cmake)
include(.cmake/sak_3rd_qt_aes.cmake)
include(.cmake/sak_3rd_nodeeditor.cmake)
include(.cmake/sak_3rd_qmdnsengine.cmake)

sak_git_get_last_commit(${CMAKE_CURRENT_SOURCE_DIR} "SAK")
sak_git_get_last_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "SAK")
sak_git_get_last_tag(${CMAKE_CURRENT_SOURCE_DIR} "SAK")

# Assistant module.
include_directories(src/assistants)
include_directories(src/assistantsui)

# Glog module.
option(SAK_USING_GLOG "Using glog module." ON)
if(SAK_USING_GLOG)
  add_compile_definitions(SAK_USING_GLOG)

  set(WITH_GTEST OFF)
  set(WITH_GFLAGS OFF)

  sak_add_subdirectory("glog-0.7.0")
endif()

sak_add_subdirectory("Qt-AES-master")
sak_add_subdirectory("qmdnsengine-master")

macro(sak_import_assistant assistant_dir_name)
  string(TOUPPER ${assistant_dir_name} upper_name)
  set(SAK_IMPORT_MODULE_NAME SAK_IMPORT_MODULE_${upper_name}ASSISTANT)
  option(${SAK_IMPORT_MODULE_NAME} "" ON)
  if(${SAK_IMPORT_MODULE_NAME})
    include_directories(${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src)
    add_compile_definitions(${SAK_IMPORT_MODULE_NAME})
    file(
      GLOB_RECURSE
      ASSISTANT_SOURCE
      "${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src/*h"
      "${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src/*.cc"
      "${CMAKE_SOURCE_DIR}/src/assistants/${assistant_dir_name}/src/*.ui")
    list(APPEND SAK_ASSISTANT_SOURCES ${ASSISTANT_SOURCE})
  endif()
endmacro()

list(APPEND SAK_ASSISTANT_SOURCES src/assistants/sakassistantsfactory.h)
list(APPEND SAK_ASSISTANT_SOURCES src/assistants/sakassistantsfactory.cc)

sak_import_assistant("crc")
sak_import_assistant("mdns")
sak_import_assistant("ping")
sak_import_assistant("ascii")
sak_import_assistant("base64")
sak_import_assistant("number")
sak_import_assistant("string")
sak_import_assistant("broadcast")
sak_import_assistant("filecheck")

option(SAK_ENABLE_MODULE_FONTAWESOMEASSISTANT "" OFF)
if(SAK_ENABLE_MODULE_FONTAWESOMEASSISTANT)
  sak_import_assistant("fontawesome")
endif()

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

sak_add_src("log")
sak_add_src("tools")
sak_add_src("common")
sak_add_src("toolbox")

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

# QtSwissArmyKnife
include_directories(src)
if(SAK_ENABLE_MODULE_FONTAWESOMEASSISTANT)
  include_directories(3rd/qt/flowlayout)
endif()
file(GLOB APP_SOURCES "src/*.h" "src/*.cc" "src/*.ui" ".cmake/*.cmake")

option(SAK_BUILD_FOR_APP_SOTRE "Build for Microsoft/Apple app store." OFF)
if(SAK_BUILD_FOR_APP_SOTRE AND CMAKE_BUILD_TYPE)
  add_definitions(-DSAK_RELEASE_FOR_APP_STORE)
  set(SAK_IMPORT_MODULE_CANBUSSTUDIO OFF)
endif()

list(APPEND SAK_APP_SOURCES ${SAK_ASSISTANT_SOURCES} ${SAK_SRC_SOURCES} ${APP_SOURCES})
list(APPEND SAK_APP_SOURCES qtswissarmyknife.qrc windows.rc)
if(SAK_IMPORT_MODULE_BLUETOOTH)
  file(GLOB SAK_BLUETOOTH_SOURCES "src/optional/bluetooth/*.h" "src/optional/bluetooth/*.cc"
       "src/optional/bluetooth/*.ui")
  list(APPEND SAK_APP_SOURCES ${SAK_BLUETOOTH_SOURCES})
  include_directories(src/optional/bluetooth)
endif()

if(SAK_ENABLE_MODULE_FONTAWESOMEASSISTANT)
  file(GLOB_RECURSE SAK_QT_FLOWLAYOUT_SOURCES "${CMAKE_SOURCE_DIR}/3rd/qt/flowlayout/*.cpp"
       "${CMAKE_SOURCE_DIR}/3rd/qt/flowlayout/*.h")
  list(APPEND SAK_APP_SOURCES ${SAK_QT_FLOWLAYOUT_SOURCES})
endif()

sak_add_executable("QtSwissArmyKnife" ${SAK_APP_SOURCES})
sak_auto_execute_deployqt(QtSwissArmyKnife)
sak_tar_target(QtSwissArmyKnife)
sak_set_target_properties(QtSwissArmyKnife)
if(SAK_ENABLE_MODULE_FONTAWESOMEASSISTANT)
  sak_import_fontawesome_for_target(QtSwissArmyKnife)
endif()

sak_3rd_setup_glog(QtSwissArmyKnife)
sak_3rd_setup_qmdnsengine(QtSwissArmyKnife)

target_link_libraries(
  QtSwissArmyKnife
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets
          Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::SerialPort
          Qt${QT_VERSION_MAJOR}::SerialBus)
if(SAK_IMPORT_MODULE_BLUETOOTH)
  target_link_libraries(QtSwissArmyKnife PRIVATE Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()

add_subdirectory(resources/translations)

install(
  TARGETS QtSwissArmyKnife
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Custom build target.
option(SAK_ENABLE_APP_ASSISTANTS "Enable to build assistants." OFF)
if(SAK_ENABLE_APP_ASSISTANTS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/assistants)
endif()

option(APP_ENABLE_CANBUSSTUDIO "Enable to build CANBusStudio." OFF)
if(APP_ENABLE_CANBUSSTUDIO)
  if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/canbusstudio)
  endif()
endif()

option(APP_ENABLE_MODBUSSTUDIO "Enable to build ModbusStudio." OFF)
if(APP_ENABLE_MODBUSSTUDIO)
  if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/modbusstudio)
  endif()
endif()

# The private module is not open source.
option(SAK_ENABLE_APP_PRIVATE "Do not change the option unless you are the author" OFF)
if(SAK_ENABLE_APP_PRIVATE)
  sak_add_subdirectory("nodeeditor-3.0.10")
  if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/private)
  endif()
endif()
