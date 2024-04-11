cmake_minimum_required(VERSION 3.21)

project(
  xTools
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(WITH_TOOLS NO)
set(WITH_GFLAGS OFF)
set(WITHOUT_PNG ON)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF)

# --------------------------------------------------------------------------------------------------
# Qt module
list(APPEND X_TOOLS_QT_COMPONENTS Gui)
list(APPEND X_TOOLS_QT_COMPONENTS Svg)
list(APPEND X_TOOLS_QT_COMPONENTS Core)
list(APPEND X_TOOLS_QT_COMPONENTS Widgets)
list(APPEND X_TOOLS_QT_COMPONENTS Network)
list(APPEND X_TOOLS_QT_COMPONENTS WebSockets)
list(APPEND X_TOOLS_QT_COMPONENTS LinguistTools)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialPort)
if(Qt${QT_VERSION_MAJOR}SerialPort_FOUND)
  option(X_TOOLS_IMPORT_MODULE_SERIALPORT "Enable SerialPort module" ON)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_SERIALPORT)
endif()
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialBus)
if(Qt${QT_VERSION_MAJOR}SerialBus_FOUND)
  option(X_TOOLS_IMPORT_MODULE_SERIALBUS "Enable SerialBus module" ON)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_SERIALBUS)
endif()

include(CMake/xToolsCommon.cmake)
include(CMake/xToolsGitInfo.cmake)
include(CMake/xToolsDeployQt.cmake)
include(CMake/xToolsThirdParty.cmake)
include(CMake/xToolsCompatibility.cmake)

option(X_TOOLS_BUILD_FOR_STORE "Build for Microsoft Store or Apple Store" OFF)
if(X_TOOLS_BUILD_FOR_STORE)
  add_compile_definitions(X_TOOLS_BUILD_FOR_STORE)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  option(X_TOOLS_IMPORT_MODULE_GLOG "Enable glog" ON)
else()
  option(X_TOOLS_IMPORT_MODULE_GLOG "Enable glog" OFF)
endif()
if(X_TOOLS_IMPORT_MODULE_GLOG)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_GLOG)
  x_tools_add_third_party("glog-0.7.0")
endif()

x_tools_git_get_latest_commit(${CMAKE_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_SOURCE_DIR} "X_TOOLS")

# --------------------------------------------------------------------------------------------------
# Private module
if(EXISTS ${CMAKE_SOURCE_DIR}/Private/Common/Source)
  option(X_TOOLS_IMPORT_MODULE_PRIVATE "Enable private module" ON)
  x_tools_add_third_party("Qt-AES-master")
  include_directories(${CMAKE_SOURCE_DIR}/Private/Common/Source)
  file(GLOB PRIVATE_COMMON_SOURCE "${CMAKE_SOURCE_DIR}/Private/Common/Source/*.*")
  list(REMOVE_ITEM PRIVATE_COMMON_SOURCE ${CMAKE_SOURCE_DIR}/Private/Common/Source/README.md)
  list(APPEND X_TOOLS_SOURCE ${PRIVATE_COMMON_SOURCE})
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_PRIVATE)
else()
  option(X_TOOLS_IMPORT_MODULE_PRIVATE "Enable private module" OFF)
endif()

# --------------------------------------------------------------------------------------------------
# BLE
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Bluetooth)
if(Qt${QT_VERSION_MAJOR}Bluetooth_FOUND)
  option(X_TOOLS_IMPORT_MODULE_BLUETOOTH "Enable Bluetooth module" ON)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_BLUETOOTH)
else()
  option(X_TOOLS_IMPORT_MODULE_BLUETOOTH "Enable Bluetooth module" OFF)
  set(X_TOOLS_BLUETOOTH_MODULE_IS_VALID OFF)
endif()
if(X_TOOLS_IMPORT_MODULE_BLUETOOTH)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_BLUETOOTH)
  set(X_TOOLS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_DIR}/Tools/Tools/xToolsBleCentralTool.h)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_DIR}/Tools/Tools/xToolsBleCentralTool.cpp)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_DIR}/Tools/ToolsUI/xToolsBleCentralToolUi.h)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_DIR}/Tools/ToolsUI/xToolsBleCentralToolUi.cpp)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_DIR}/Tools/ToolsUI/xToolsBleCentralToolUi.ui)
endif()

# --------------------------------------------------------------------------------------------------
# Modbus, CAN BUS
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialBus)
if(Qt${QT_VERSION_MAJOR}SerialBus_FOUND)
  option(X_TOOLS_IMPORT_MODULE_SERIALBUS "Enable SerialBus module" ON)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_SERIALBUS)

  option(X_TOOLS_IMPORT_MODULE_MODBUS "Enable Modbus module" ON)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_MODBUS)

  option(X_TOOLS_IMPORT_MODULE_CANBUS "Enable Modbus module" ON)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_CANBUS)
else()
  option(X_TOOLS_IMPORT_MODULE_MODBUS "Enable Modbus module" OFF)
  option(X_TOOLS_IMPORT_MODULE_CANBUS "Enable CANBus module" OFF)
endif()
if(X_TOOLS_IMPORT_MODULE_MODBUS)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_MODBUS)
  set(X_TOOLS_MODBUS_DIR ${CMAKE_SOURCE_DIR}/Source/Modbus/Source)
  file(GLOB MODBUS_SOURCE "${X_TOOLS_MODBUS_DIR}/*.*")
  list(REMOVE_ITEM MODBUS_SOURCE ${X_TOOLS_MODBUS_DIR}/main.cpp)
  list(APPEND X_TOOLS_SOURCE ${MODBUS_SOURCE})
  include_directories(${CMAKE_SOURCE_DIR}/Source/Modbus/Source)
endif()
if(X_TOOLS_IMPORT_MODULE_CANBUS)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_CANBUS)
  set(X_TOOLS_CANBUS_DIR ${CMAKE_SOURCE_DIR}/Source/CANBus/Source)
  file(GLOB CANBUS_SOURCE "${X_TOOLS_CANBUS_DIR}/*.*")
  list(REMOVE_ITEM CANBUS_SOURCE ${X_TOOLS_CANBUS_DIR}/main.cpp)
  list(APPEND X_TOOLS_SOURCE ${CANBUS_SOURCE})
  include_directories(${CMAKE_SOURCE_DIR}/Source/CANBus/Source)
endif()

# --------------------------------------------------------------------------------------------------
# Common module
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/Common)
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/CommonUI)
add_subdirectory(Source/Common)

# --------------------------------------------------------------------------------------------------
# Assistant module
set(X_TOOLS_ASSISTANT_DIR "${CMAKE_SOURCE_DIR}/Source/Assistants")
macro(x_tools_add_assistant dir_name on_off)
  string(TOUPPER ${dir_name} DIR_NAME_UPPER)
  option(X_TOOLS_IMPORT_MODULE_${DIR_NAME_UPPER} "Enable ${dir_name} assistant" ${on_off})
  if(X_TOOLS_IMPORT_MODULE_${DIR_NAME_UPPER})
    include_directories(${X_TOOLS_ASSISTANT_DIR}/${dir_name}/Source)
    add_compile_definitions(X_TOOLS_IMPORT_MODULE_${DIR_NAME_UPPER})
    file(GLOB ASSISTANT_SOURCE "${X_TOOLS_ASSISTANT_DIR}/${dir_name}/Source/*.*")
    list(REMOVE_ITEM ASSISTANT_SOURCE ${X_TOOLS_ASSISTANT_DIR}/${dir_name}/Source/main.cpp)
    list(APPEND X_TOOLS_SOURCE ${ASSISTANT_SOURCE})
  endif()
endmacro()

option(X_TOOLS_IMPORT_MODULE_ASSISTANTS "Enable assistants module" ON)
if(X_TOOLS_IMPORT_MODULE_ASSISTANTS)
  x_tools_add_assistant("CRC" ON)
  x_tools_add_assistant("mDNS" ON)
  x_tools_add_assistant("Ping" ON)
  x_tools_add_assistant("ASCII" ON)
  x_tools_add_assistant("Base64" ON)
  x_tools_add_assistant("Number" ON)
  x_tools_add_assistant("String" ON)
  x_tools_add_assistant("QRCode" OFF)
  x_tools_add_assistant("Broadcast" ON)
  x_tools_add_assistant("FileCheck" ON)
  x_tools_add_assistant("FileMerge" ON)

  if(X_TOOLS_IMPORT_MODULE_QRCODE)
    x_tools_add_third_party("libqrencode-master")
  endif()

  if(X_TOOLS_IMPORT_MODULE_MDNS)
    x_tools_add_third_party("qmdnsengine-master")
  endif()

  list(APPEND X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Assistants/xToolsAssistantFactory.h)
  list(APPEND X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Assistants/xToolsAssistantFactory.cpp)
  include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_ASSISTANTS)
endif()

# --------------------------------------------------------------------------------------------------
# Tools
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/Tools)
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBox)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBoxUI)
file(GLOB X_TOOLS_SOURCE_TMP "${CMAKE_SOURCE_DIR}/Source/*.*")
file(GLOB X_TOOLS_CMAKE_FILES "${CMAKE_SOURCE_DIR}/CMake/*.cmake")
file(GLOB_RECURSE X_TOOLS_TOOLS_SOURCE "${CMAKE_SOURCE_DIR}/Source/Tools/*.*")
file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE "${CMAKE_SOURCE_DIR}/Source/ToolBox/*.*")
list(APPEND X_TOOLS_SOURCE xTools.rc)
list(APPEND X_TOOLS_SOURCE xTools.qrc)
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_TMP})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_CMAKE_FILES})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLS_SOURCE})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLBOX_SOURCE})

# --------------------------------------------------------------------------------------------------
# xTools application
x_tools_add_executable(xTools ${X_TOOLS_SOURCE})
x_tools_generate_translations(xTools)
x_tools_deploy_qt(xTools)
if(WIN32)
  x_tools_tar_target(xTools)
endif()
if(X_TOOLS_ENABLE_ADVANCED_STYLESHEET)
  x_tools_copy_style_resources_for_target(xTools)
endif()

if(X_TOOLS_IMPORT_MODULE_QRCODE)
  target_link_libraries(xTools PRIVATE qrencode)
endif()
if(X_TOOLS_IMPORT_MODULE_GLOG)
  target_link_libraries(xTools PRIVATE glog::glog)
endif()
if(X_TOOLS_IMPORT_MODULE_MDNS)
  target_link_libraries(xTools PRIVATE qmdnsengine)
endif()
target_link_libraries(xTools PRIVATE xToolsCommon)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Svg)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)
if(X_TOOLS_IMPORT_MODULE_SERIALPORT)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
endif()
if(X_TOOLS_IMPORT_MODULE_SERIALBUS)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::SerialBus)
endif()
if(X_TOOLS_IMPORT_MODULE_BLUETOOTH)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()
if(X_TOOLS_IMPORT_MODULE_PRIVATE)
  target_link_libraries(xTools PRIVATE QtAES::QtAES)
endif()

option(X_TOOLS_ENABLE_GIT_COMMAND "Some git commands for pulling and pushing" OFF)
if(X_TOOLS_ENABLE_GIT_COMMAND)
  add_custom_target(
    xTools_pull
    COMMAND git pull "https://gitee.com/x-tools-author/x-tools-private.git"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  add_custom_target(
    xTools_push
    COMMAND git push
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

# -------------------------------------------------------------------------------------------------
# Assistant applications
option(X_TOOLS_ENABLE_APP_ASSISTANTS "Enable assistant applications" OFF)
if(X_TOOLS_ENABLE_APP_ASSISTANTS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Assistants)
endif()

# -------------------------------------------------------------------------------------------------
# The private modules is not open-source.
if((QT_VERSION_MAJOR GREATER 5) AND (NOT Qt6_VERSION VERSION_LESS "6.5.0"))
  if(EXISTS ${CMAKE_SOURCE_DIR}/Private)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Private)
  endif()

  if(X_TOOLS_ENABLE_GIT_COMMAND)
    set(GITEE_URL "https://gitee.com/x-tools-author/x-tools-private.git")
    add_custom_target(
      xTools_clone_private_modules
      COMMAND git clone ${GITEE_URL} ./Private || echo "clone private modules"
      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    add_custom_target(
      xTools_pull_private_modules
      COMMAND git pull ${GITEE_URL}
      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Private)
    add_custom_target(
      xTools_push_private_modules
      COMMAND git push
      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Private)
  endif()
endif()
