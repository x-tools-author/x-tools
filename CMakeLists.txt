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

option(X_TOOLS_BUILD_FOR_STORE "Build for Microsoft Store or Apple Store" OFF)
if(X_TOOLS_BUILD_FOR_STORE)
  add_compile_definitions(X_TOOLS_BUILD_FOR_STORE)
endif()

set(X_TOOLS_QT_COMPONENTS Core Gui Widgets WebSockets Network LinguistTools)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  list(APPEND X_TOOLS_QT_COMPONENTS SerialPort)
  list(APPEND X_TOOLS_QT_COMPONENTS SerialBus)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_SERIALPORT)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_SERIALBUS)
endif()

add_compile_definitions(X_TOOLS_BUILD_WITH_CMAKE)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})

include(CMake/xToolsCommon.cmake)
include(CMake/xToolsGitInfo.cmake)
include(CMake/xToolsDeployQt.cmake)

x_tools_git_get_latest_commit(${CMAKE_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_SOURCE_DIR} "X_TOOLS")

# --------------------------------------------------------------------------------------------------
# Common module
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/Common)
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/CommonUI)
if(QT_VERSION_MAJOR EQUAL 6)
  option(X_TOOLS_IMPORT_MODULE_BLUETOOTH "Enable Bluetooth modbule" ON)
else()
  option(X_TOOLS_IMPORT_MODULE_BLUETOOTH "Enable Bluetooth modbule" OFF)
endif()
set(X_TOOLS_BLUETOOTH_MODULE_IS_VALID FALSE)
if(X_TOOLS_IMPORT_MODULE_BLUETOOTH AND QT_VERSION_MAJOR GREATER 5)
  if(NOT Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS "6.5.0")
    find_package(
      Qt${QT_VERSION_MAJOR} REQUIRED
      COMPONENTS Bluetooth
      QUIET)
    if(Qt${QT_VERSION_MAJOR}Bluetooth_FOUND)
      add_compile_definitions(X_TOOLS_IMPORT_MODULE_BLUETOOTH)
      set(X_TOOLS_BLUETOOTH_MODULE_IS_VALID TRUE)
    endif()
  endif()
endif()
add_subdirectory(Source/Common)

file(GLOB_RECURSE X_TOOLS_TOOLS_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/Tools/*.h")
file(GLOB_RECURSE X_TOOLS_TOOLS_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/Tools/*.ui")
file(GLOB_RECURSE X_TOOLS_TOOLS_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/Tools/*.cpp")

file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/ToolBox/*.h")
file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/ToolBox/*.ui")
file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/ToolBox/*.cpp")

file(GLOB X_TOOLS_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/*.h")
file(GLOB X_TOOLS_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/*.ui")
file(GLOB X_TOOLS_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/*.cpp")

file(GLOB X_TOOLS_ASSISTANT_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/Assistants/*/Source/*.h")
file(GLOB X_TOOLS_ASSISTANT_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/Assistants/*/Source/*.ui")
file(GLOB X_TOOLS_ASSISTANT_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/Assistants/*/Source/*.cpp")

file(GLOB X_TOOLS_CMAKE_FILES "${CMAKE_SOURCE_DIR}/CMake/*.cmake")

list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLS_SOURCE_H})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLS_SOURCE_UI})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLS_SOURCE_CPP})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLBOX_SOURCE_H})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLBOX_SOURCE_UI})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLBOX_SOURCE_CPP})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_H})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_UI})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_SOURCE_CPP})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_ASSISTANT_SOURCE_H})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_ASSISTANT_SOURCE_UI})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_ASSISTANT_SOURCE_CPP})
list(APPEND X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Assistants/xToolsAssistantFactory.h)
list(APPEND X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Assistants/xToolsAssistantFactory.cpp)
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_CMAKE_FILES})
list(APPEND X_TOOLS_SOURCE xTools.qrc)
# list(APPEND X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/xToolsCommon.qrc)
if(WIN32)
  list(APPEND X_TOOLS_SOURCE xTools.rc)
endif()

include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants)
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/Tools)
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBox)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBoxUI)

include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/CRC/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/mDNS/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/Ping/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/ASCII/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/Base64/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/Number/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/String/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/Broadcast/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/FileCheck/Source)

# -------------------------------------------------------------------------------------------------
# Modbus module
option(X_TOOLS_IMPORT_MODULE_MODBUS_STUDIO "Enable Modbus Studio module" ON)
if(X_TOOLS_IMPORT_MODULE_MODBUS_STUDIO)
  file(GLOB_RECURSE X_TOOLS_MODBUS_STUDIO_H "${CMAKE_SOURCE_DIR}/Source/ModbusStudio/*.h")
  file(GLOB_RECURSE X_TOOLS_MODBUS_STUDIO_UI "${CMAKE_SOURCE_DIR}/Source/ModbusStudio/*.ui")
  file(GLOB_RECURSE X_TOOLS_MODBUS_STUDIO_CPP "${CMAKE_SOURCE_DIR}/Source/ModbusStudio/*.cpp")
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_MODBUS_STUDIO)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_MODBUS_STUDIO_H})
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_MODBUS_STUDIO_UI})
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_MODBUS_STUDIO_CPP})
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/ModbusStudio/main.cpp)
  include_directories(${CMAKE_SOURCE_DIR}/Source/ModbusStudio/ModbusStudio)
  include_directories(${CMAKE_SOURCE_DIR}/Source/ModbusStudio/ModbusStudioUI)
endif()

# -------------------------------------------------------------------------------------------------
# CAN bus module

option(X_TOOLS_IMPORT_MODULE_CANBUS_STUDIO "Enable CAN Bus module" ON)
if(X_TOOLS_IMPORT_MODULE_CANBUS_STUDIO)
  file(GLOB_RECURSE X_TOOLS_CANBUS_STUDIO_H "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.h")
  file(GLOB_RECURSE X_TOOLS_CANBUS_STUDIO_UI "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.ui")
  file(GLOB_RECURSE X_TOOLS_CANBUS_STUDIO_CPP "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.cpp")
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_CANBUS_STUDIO)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_CANBUS_STUDIO_H})
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_CANBUS_STUDIO_UI})
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_CANBUS_STUDIO_CPP})
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/CANBusStudio/main.cpp)
  include_directories(${CMAKE_SOURCE_DIR}/Source/CANBusStudio/CANBusStudio)
  include_directories(${CMAKE_SOURCE_DIR}/Source/CANBusStudio/CANBusStudioUI)
endif()

# -------------------------------------------------------------------------------------------------
# BLE module
if(NOT X_TOOLS_BLUETOOTH_MODULE_IS_VALID)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Tools/Tools/xToolsBleCentralTool.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Tools/Tools/xToolsBleCentralTool.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI/xToolsBleCentralToolUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE
       ${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI/xToolsBleCentralToolUi.cpp)
  message(STATUS "The xTools BLE module source has been removed!")
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
set(WITH_GFLAGS OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF)

x_tools_add_third_party("glog-0.7.0")
x_tools_add_third_party("qmdnsengine-master")
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Private)

x_tools_add_executable(xTools ${X_TOOLS_SOURCE})
x_tools_generate_translations(xTools)
x_tools_deploy_qt(xTools)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_definitions(X_TOOLS_USING_GLOG)
endif()

target_link_libraries(xTools PRIVATE glog::glog)
target_link_libraries(xTools PRIVATE qmdnsengine)
target_link_libraries(xTools PRIVATE xToolsCommon)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::SerialBus)
endif()
if(X_TOOLS_BLUETOOTH_MODULE_IS_VALID)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()

# -------------------------------------------------------------------------------------------------
# Other applications
option(X_TOOLS_ENABLE_APP_ASSISTANTS "Enable assistant applications" OFF)
if(X_TOOLS_ENABLE_APP_ASSISTANTS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Assistants)
endif()

option(X_TOOLS_ENABLE_APP_MODBUS_STUDIO "Enable Modbus Studio application" OFF)
if(X_TOOLS_ENABLE_APP_MODBUS_STUDIO)
  add_subdirectory(${CMAKE_SOURCE_DIR}/Source/ModbusStudio)
endif()

option(X_TOOLS_ENABLE_APP_CANBUS_STUDIO "Enable CANBus Studio application" OFF)
if(X_TOOLS_ENABLE_APP_CANBUS_STUDIO)
  add_subdirectory(${CMAKE_SOURCE_DIR}/Source/CANBusStudio)
endif()
