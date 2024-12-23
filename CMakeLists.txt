cmake_minimum_required(VERSION 3.21)

# cmake-format: off
project(xTools VERSION 1.0 LANGUAGES C CXX)
# cmake-format: on

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# cmake-format: off
set(WITH_GFLAGS OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Reset option" FORCE)
set(WITH_TOOLS OFF CACHE BOOL "Reset option" FORCE)
# cmake-format: on

# --------------------------------------------------------------------------------------------------
# Qt module
list(APPEND X_TOOLS_QT_COMPONENTS Gui)
list(APPEND X_TOOLS_QT_COMPONENTS Svg)
list(APPEND X_TOOLS_QT_COMPONENTS Core)
list(APPEND X_TOOLS_QT_COMPONENTS Widgets)
list(APPEND X_TOOLS_QT_COMPONENTS Network)
list(APPEND X_TOOLS_QT_COMPONENTS LinguistTools)

# Qt6 is preferred, but Qt5 is also supported
find_package(QT NAMES Qt6 QUIET COMPONENTS ${X_TOOLS_QT_COMPONENTS})
if(NOT QT_FOUND)
  find_package(QT NAMES Qt5 REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})
endif()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})

if(QT_VERSION VERSION_LESS 6.8.0)
  message(FATAL_ERROR "Qt 6.8.0 or later is required")
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
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/QIFW/QIFW.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/xToolsCommon.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/xToolsGitInfo.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/xToolsDeployQt.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/xToolsThirdParty.cmake)

# --------------------------------------------------------------------------------------------------
# Git module
set(GIT_TAG "x.x.x")
x_tools_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")

# --------------------------------------------------------------------------------------------------
# Get all source files of the project
file(GLOB_RECURSE X_TOOLS_SOURCE_H "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE X_TOOLS_SOURCE_UI "${CMAKE_CURRENT_SOURCE_DIR}/source/*.ui")
file(GLOB_RECURSE X_TOOLS_SOURCE_CPP "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
set(X_TOOLS_SOURCE ${X_TOOLS_SOURCE_H} ${X_TOOLS_SOURCE_UI} ${X_TOOLS_SOURCE_CPP})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

# --------------------------------------------------------------------------------------------------
# Third party module
x_tools_add_third_party("glog-0.7.1")
x_tools_add_third_party("libqrencode-master")
x_tools_add_third_party("qmdnsengine-master")

# Custom stylesheet
if(NOT X_TOOLS_ENABLE_MODULE_STYLESHEET)
  message(STATUS "Stylesheet module is disable, stylesheet files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/Common/StyleSheetManager.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/Common/StyleSheetManager.cpp)
endif()

# HID
option(X_TOOLS_ENABLE_MODULE_HID "Enable HID module" OFF)
if(X_TOOLS_ENABLE_MODULE_HID)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_HID)
  set(HIDAPI_WITH_LIBUSB FALSE)
  x_tools_add_third_party("hidapi-hidapi-0.14.0")
endif()
if(NOT X_TOOLS_ENABLE_MODULE_HID)
  message(STATUS "HID module is disable, HID files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/Hid.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/Hid.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/HidUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/HidUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/HidUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidStyledItemDelegate.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidStyledItemDelegate.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.cpp)
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
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/Unit/SerialPortScanner.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/Unit/SerialPortScanner.cpp)
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
# Qt SerialPort module
option(X_TOOLS_ENABLE_MODULE_WEB_SOCKET "Enable WebSockets module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS WebSockets)
if(NOT Qt${QT_VERSION_MAJOR}WebSockets_FOUND)
  set(X_TOOLS_ENABLE_MODULE_WEB_SOCKET OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_WEB_SOCKET)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_WEB_SOCKET)
else()
  message(STATUS "SerialPort module is disable, serial port files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketClient.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketClient.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketClientUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketClientUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketClientUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.cpp)

  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketServer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketServer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketServerUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketServerUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketServerUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.cpp)
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
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
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
if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/Modbus/source)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_MODBUS)

  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_MODBUS_DIR}/main.cpp)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/CANBus/source)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_CANBUS)

  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_CANBUS_DIR}/main.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Qt Charts module
option(X_TOOLS_ENABLE_MODULE_CHARTS "Enable Charts module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Charts)
if(NOT Qt${QT_VERSION_MAJOR}Charts_FOUND)
  set(X_TOOLS_ENABLE_MODULE_CHARTS OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_CHARTS)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_CHARTS)
  x_tools_add_xlsx()
else()
  message(STATUS "Charts module is disable, chart files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/IO)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/DataVisualization/2D/Charts.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/DataVisualization/2D/Charts.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUiSettings.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUiSettings.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUiSettings.ui)
endif()

# --------------------------------------------------------------------------------------------------
# Tools
set(X_TOOLS_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/Tools")
macro(add_tool dir_name)
  file(GLOB TOOL_SOURCE "${X_TOOLS_TOOLS_DIR}/${dir_name}/*.*")
  include_directories(${X_TOOLS_TOOLS_DIR}/${dir_name})
  list(APPEND X_TOOLS_SOURCE ${TOOL_SOURCE})
  # away remove main.cpp
  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_TOOLS_DIR}/${dir_name}/main.cpp)
endmacro()
# cmake-format: off
file(GLOB ALL_TOOLS RELATIVE ${X_TOOLS_TOOLS_DIR} ${X_TOOLS_TOOLS_DIR}/*)
list(REMOVE_ITEM ALL_TOOLS "AssistantFactory.h")
list(REMOVE_ITEM ALL_TOOLS "AssistantFactory.cpp")
# cmake-format: on
if(NOT X_TOOLS_ENABLE_MODULE_SERIALBUS)
  list(REMOVE_ITEM ALL_TOOLS "CANBus" "Modbus")
endif()

foreach(tool ${ALL_TOOLS})
  add_tool(${tool})
endforeach()

set(X_TOOLS_TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/Tools)
include_directories(${X_TOOLS_TOOLS_DIR})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLS_DIR}/AssistantFactory.h)
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLS_DIR}/AssistantFactory.cpp)

# --------------------------------------------------------------------------------------------------
# xTools Core
list(APPEND X_TOOLS_SOURCE xTools.qrc)
list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp)
add_library(xToolsCore STATIC ${X_TOOLS_SOURCE})
x_tools_generate_translations(xToolsCore)
x_tools_add_stylesheet_sources(xToolsCore)

target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(xToolsCore PUBLIC glog::glog qrencode qmdnsengine)

if(X_TOOLS_ENABLE_MODULE_SERIAL_PORT)
  target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::SerialPort)
endif()

if(X_TOOLS_ENABLE_MODULE_WEB_SOCKET)
  target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::WebSockets)
endif()

if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::SerialBus)
endif()

if(X_TOOLS_ENABLE_MODULE_BLUETOOTH)
  target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()

if(X_TOOLS_ENABLE_MODULE_CHARTS)
  target_link_libraries(xToolsCore PUBLIC Qt${QT_VERSION_MAJOR}::Charts QXlsx)
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
option(X_TOOLS_ENABLE_TARGET_XTOOLS "Enable xTools applications" ON)
if(X_TOOLS_ENABLE_TARGET_XTOOLS)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${X_TOOLS_BINARY_DIR}/xTools)

  if(CMAKE_HOST_APPLE)
    set(MACOSX_BUNDLE_ICON_FILE xTools.icns)
    set(xTools_ICON ${CMAKE_SOURCE_DIR}/xTools.icns)
    set_source_files_properties(${xTools_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  endif()

  qt_add_executable(
    xTools
    ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    xTools.rc
    android/AndroidManifest.xml
    android/res/values/libs.xml
    android/res/xml/qtprovider_paths.xml
    ${xTools_ICON})
  target_link_libraries(xTools PRIVATE xToolsCore)
  set_target_properties(
    xTools
    PROPERTIES # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE
               QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)

  x_tools_add_stylesheet_resources(xTools)
  x_tools_deploy_qt(xTools)
  x_tools_tar_target(xTools)

  # remove "v" from version
  set(version ${GIT_TAG})
  string(SUBSTRING ${version} 1 -1 version)
  x_tools_generate_installer(xTools ${version} ${CMAKE_CURRENT_SOURCE_DIR}/xTools.ico)
endif()

# --------------------------------------------------------------------------------------------------
# Assistant applications
macro(add_tool_app dir_name app_name)
  set(TOOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/Tools/${dir_name}/main.cpp)
  if(WIN32)
    list(APPEND TOOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.rc)
  endif()

  x_tools_add_executable(${app_name} ${TOOL_SOURCES})
  x_tools_deploy_qt(${app_name})
  target_link_libraries(${app_name} PRIVATE xToolsCore)
endmacro()

option(X_TOOLS_ENABLE_TARGET_TOOLS "Enable tool applications" OFF)
if(X_TOOLS_ENABLE_TARGET_TOOLS)
  foreach(tool ${ALL_TOOLS})
    add_tool_app(${tool} "${tool}Assistant")
  endforeach()
endif()
