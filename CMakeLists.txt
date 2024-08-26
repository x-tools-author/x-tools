cmake_minimum_required(VERSION 3.21)

project(
  xTools
  VERSION 1.0
  LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(WITH_GFLAGS OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL "Reset option" FORCE)
set(WITH_TOOLS
    OFF
    CACHE BOOL "Reset option" FORCE)

# --------------------------------------------------------------------------------------------------
# Qt module
list(APPEND X_TOOLS_QT_COMPONENTS Gui)
list(APPEND X_TOOLS_QT_COMPONENTS Svg)
list(APPEND X_TOOLS_QT_COMPONENTS Core)
list(APPEND X_TOOLS_QT_COMPONENTS Widgets)
list(APPEND X_TOOLS_QT_COMPONENTS Network)
list(APPEND X_TOOLS_QT_COMPONENTS WebSockets)
list(APPEND X_TOOLS_QT_COMPONENTS LinguistTools)

# Qt6 is preferred, but Qt5 is also supported
find_package(QT NAMES Qt6 QUIET COMPONENTS ${X_TOOLS_QT_COMPONENTS})
if(NOT QT_FOUND)
  find_package(QT NAMES Qt5 REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})
endif()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})

if(QT_VERSION VERSION_LESS 6.8.0)
  message(WARNING "Qt 6.8.0 or later is recommended")
endif()

# Don't change X_TOOLS_STATIC_VC_RUNTIME_LIBRARY option unless you know what you are doing.
# QT_TARGET_TYPE is SHARED_LIBRARY or STATIC_LIBRARY
get_target_property(QT_TARGET_TYPE Qt${QT_VERSION_MAJOR}::Core TYPE)
option(X_TOOLS_STATIC_VC_RUNTIME_LIBRARY "Using static vc runtime library" OFF)
if(WIN32 AND MSVC)
  if(QT_TARGET_TYPE STREQUAL "STATIC_LIBRARY" AND X_TOOLS_STATIC_VC_RUNTIME_LIBRARY)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# CMake module
include(${CMAKE_CURRENT_LIST_DIR}/CMake/QIFW/QIFW.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMake/xToolsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMake/xToolsGitInfo.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMake/xToolsDeployQt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMake/xToolsThirdParty.cmake)

# --------------------------------------------------------------------------------------------------
# Git module
# cmake-format: off
x_tools_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")

set(X_TOOLS_VERSION ${GIT_TAG} CACHE STRING "The version of applicaion")
# cmake-format: on

# --------------------------------------------------------------------------------------------------
# Google log module
x_tools_add_third_party("glog-0.7.0")

# --------------------------------------------------------------------------------------------------
# Get all source files of the project
file(GLOB_RECURSE X_TOOLS_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/*.h")
file(GLOB_RECURSE X_TOOLS_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/*.ui")
file(GLOB_RECURSE X_TOOLS_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/*.cpp")
set(X_TOOLS_SOURCE ${X_TOOLS_SOURCE_H} ${X_TOOLS_SOURCE_UI} ${X_TOOLS_SOURCE_CPP})

include_directories(${CMAKE_CURRENT_LIST_DIR}/Source)

# --------------------------------------------------------------------------------------------------
# Third party module
# Qt-Advanced-Stylesheets-main：https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets
if(NOT X_TOOLS_ENABLE_MODULE_STYLESHEET)
  message(STATUS "Stylesheet module is disable, stylesheet files will be removed.")
  set(TMP_DIR ${CMAKE_SOURCE_DIR}/Source/Common)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/CommonUI/xToolsStyleSheetManager.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/CommonUI/xToolsStyleSheetManager.cpp)
endif()

if(NOT X_TOOLS_ENABLE_MODULE_HID)
  message(STATUS "HID module is disable, HID files will be removed.")
  set(TMP_DIR ${CMAKE_SOURCE_DIR}/Source/Tools)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/Tools/xToolsHidTool.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/Tools/xToolsHidTool.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/Tools/xToolsHidManager.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/Tools/xToolsHidManager.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/ToolsUI/xToolsHidToolUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/ToolsUI/xToolsHidToolUi.h)
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialPort module
option(X_TOOLS_ENABLE_MODULE_SERIAL_PORT "Enable SerialPort module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialPort)
if(NOT Qt${QT_VERSION_MAJOR}SerialPort_FOUND)
  set(X_TOOLS_ENABLE_MODULE_SERIAL_PORT OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIAL_PORT)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_SERIAL_PORT)
else()
  message(STATUS "SerialPort module is disable, serial port files will be removed.")
  set(TMP_DIR ${CMAKE_SOURCE_DIR}/Source)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/SerialPort.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/SerialPort.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/SerialPortUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/SerialPortUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/SerialPortUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortStyledItemDelegate.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortStyledItemDelegate.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# BLE
option(X_TOOLS_ENABLE_MODULE_BLUETOOTH "Enable Bluetooth module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Bluetooth)
if((NOT Qt${QT_VERSION_MAJOR}Bluetooth_FOUND) OR (QT_VERSION VERSION_LESS 6.5.0))
  set(X_TOOLS_ENABLE_MODULE_BLUETOOTH OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_BLUETOOTH)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_BLUETOOTH)
else()
  message(STATUS "Bluetooth module is disable, bluetooth files will be removed.")
  set(TMP_DIR ${CMAKE_SOURCE_DIR}/Source)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/Unit/BleScanner.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/Unit/BleScanner.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/BleCentral.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/BleCentral.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/BleCentralUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/BleCentralUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/BleCentralUi.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialBus module
option(X_TOOLS_ENABLE_MODULE_SERIALBUS "Enable SerialBus module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialBus)
if(NOT Qt${QT_VERSION_MAJOR}SerialBus_FOUND)
  set(X_TOOLS_ENABLE_MODULE_SERIALBUS OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_SERIALBUS)
endif()

option(X_TOOLS_ENABLE_MODULE_SERIALBUS "Enable SerialBus module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialBus)
if(NOT Qt${QT_VERSION_MAJOR}SerialBus_FOUND)
  set(X_TOOLS_ENABLE_MODULE_SERIALBUS OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_SERIALBUS)
endif()

option(X_TOOLS_ENABLE_MODULE_MODBUS "Enable Modbus module" ON)
option(X_TOOLS_ENABLE_MODULE_CANBUS "Enable CAN bus module" ON)
if(NOT X_TOOLS_ENABLE_MODULE_SERIALBUS)
  set(X_TOOLS_ENABLE_MODULE_MODBUS OFF)
  set(X_TOOLS_ENABLE_MODULE_CANBUS OFF)
endif()

if(X_TOOLS_ENABLE_MODULE_SERIALBUS AND X_TOOLS_ENABLE_MODULE_MODBUS)
  message(STATUS "Modbus module is enable, modbus files will be added.")
  include_directories(${CMAKE_SOURCE_DIR}/Source/Modbus/Source)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_MODBUS)

  set(X_TOOLS_MODBUS_DIR ${CMAKE_SOURCE_DIR}/Source/Modbus/Source)
  file(GLOB MODBUS_SOURCE "${X_TOOLS_MODBUS_DIR}/*.*")
  list(APPEND X_TOOLS_SOURCE ${MODBUS_SOURCE})
  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_MODBUS_DIR}/main.cpp)
endif()

if(X_TOOLS_ENABLE_MODULE_SERIALBUS AND X_TOOLS_ENABLE_MODULE_CANBUS)
  message(STATUS "CAN bus module is enable, CAN bus files will be added.")
  include_directories(${CMAKE_SOURCE_DIR}/Source/CANBus/Source)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_CANBUS)

  set(X_TOOLS_CANBUS_DIR ${CMAKE_SOURCE_DIR}/Source/CANBus/Source)
  file(GLOB CANBUS_SOURCE "${X_TOOLS_CANBUS_DIR}/*.*")
  list(APPEND X_TOOLS_SOURCE ${CANBUS_SOURCE})
  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_CANBUS_DIR}/main.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Assistant module
set(X_TOOLS_ASSISTANT_DIR "${CMAKE_SOURCE_DIR}/Source/Tools")
macro(x_tools_add_assistant dir_name on_off)
  string(TOUPPER ${dir_name} DIR_NAME_UPPER)
  option(X_TOOLS_ENABLE_MODULE_${DIR_NAME_UPPER} "Enable ${dir_name} assistant" ${on_off})

  file(GLOB ASSISTANT_SOURCE "${X_TOOLS_ASSISTANT_DIR}/${dir_name}/Source/*.*")
  if(X_TOOLS_ENABLE_MODULE_${DIR_NAME_UPPER})
    include_directories(${X_TOOLS_ASSISTANT_DIR}/${dir_name}/Source)
    add_compile_definitions(X_TOOLS_ENABLE_MODULE_${DIR_NAME_UPPER})
    list(APPEND X_TOOLS_SOURCE ${ASSISTANT_SOURCE})
  else()
    foreach(SOURCE_FILE ${ASSISTANT_SOURCE})
      list(REMOVE_ITEM X_TOOLS_SOURCE ${SOURCE_FILE})
    endforeach()
  endif()

  # away remove main.cpp
  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_ASSISTANT_DIR}/${dir_name}/Source/main.cpp)
endmacro()

option(X_TOOLS_ENABLE_MODULE_ASSISTANTS "Enable assistants module" ON)
if(X_TOOLS_ENABLE_MODULE_ASSISTANTS)
  x_tools_add_assistant("CRC" ON)
  x_tools_add_assistant("mDNS" ON)
  x_tools_add_assistant("Ping" ON)
  x_tools_add_assistant("ASCII" ON)
  x_tools_add_assistant("Base64" ON)
  x_tools_add_assistant("Number" ON)
  x_tools_add_assistant("String" ON)
  x_tools_add_assistant("QRCode" ON)
  x_tools_add_assistant("Broadcast" ON)
  x_tools_add_assistant("FileCheck" ON)
  x_tools_add_assistant("FileMerge" ON)
  x_tools_add_assistant("CANBus" ON)
  x_tools_add_assistant("Modbus" ON)

  if(X_TOOLS_ENABLE_MODULE_QRCODE)
    x_tools_add_third_party("libqrencode-master")
  endif()

  if(X_TOOLS_ENABLE_MODULE_MDNS)
    x_tools_add_third_party("qmdnsengine-master")
  endif()

  set(X_TOOLS_ASSISTANT_DIR ${CMAKE_CURRENT_LIST_DIR}/Source/Tools)
  include_directories(${X_TOOLS_ASSISTANT_DIR})
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_ASSISTANTS)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_ASSISTANT_DIR}/AssistantFactory.h)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_ASSISTANT_DIR}/AssistantFactory.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
option(X_TOOLS_ENABLE_TARGET_XTOOLS "Enable xTools applications" ON)
if(X_TOOLS_ENABLE_TARGET_XTOOLS)
  list(APPEND X_TOOLS_SOURCE xTools.qrc xTools.rc)

  x_tools_add_executable(xTools ${X_TOOLS_SOURCE})
  x_tools_setup_stylesheet(xTools)
  x_tools_generate_translations(xTools)
  x_tools_deploy_qt(xTools)
  x_tools_tar_target(xTools)

  set(QtX Qt${QT_VERSION_MAJOR})
  target_link_libraries(xTools PRIVATE ${QtX}::Core ${QtX}::Gui ${QtX}::Svg ${QtX}::Widgets)
  target_link_libraries(xTools PRIVATE ${QtX}::Network ${QtX}::WebSockets)
  target_link_libraries(xTools PUBLIC glog::glog)

  if(X_TOOLS_ENABLE_MODULE_QRCODE)
    target_link_libraries(xTools PUBLIC qrencode)
  endif()

  if(X_TOOLS_ENABLE_MODULE_MDNS)
    target_link_libraries(xTools PUBLIC qmdnsengine)
  endif()

  if(X_TOOLS_ENABLE_MODULE_SERIAL_PORT)
    target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::SerialPort)
  endif()

  if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
    target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::SerialBus)
  endif()

  if(X_TOOLS_ENABLE_MODULE_BLUETOOTH)
    target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::Bluetooth)
  endif()

  option(X_TOOLS_ENABLE_TARGET_XTOOLS_INSTALLER "Enable xTools applications" OFF)
  if(X_TOOLS_ENABLE_TARGET_XTOOLS_INSTALLER)
    x_tools_generate_installer(xTools ${X_TOOLS_VERSION})
  endif()
endif()

# -------------------------------------------------------------------------------------------------
# Assistant applications
option(X_TOOLS_ENABLE_TARGET_TOOLS "Enable tool applications" OFF)
if(X_TOOLS_ENABLE_TARGET_TOOLS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Tools)
endif()
