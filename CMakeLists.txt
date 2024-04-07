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
if(NOT Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS "5.14.0")
  add_compile_definitions(X_TOOLS_ENABLE_HIGH_DPI_POLICY)
endif()

include(CMake/xToolsCompatibility.cmake)
include(CMake/xToolsCommon.cmake)
include(CMake/xToolsGitInfo.cmake)
include(CMake/xToolsDeployQt.cmake)

set(WITH_TOOLS OFF)
set(WITH_GFLAGS OFF)
set(WITHOUT_PNG OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_definitions(X_TOOLS_USING_GOOGLE_LOG)
endif()
add_compile_definitions(X_TOOLS_IMPORT_MODULE_MDNS)
add_compile_definitions(X_TOOLS_IMPORT_MODULE_QRCODE)

x_tools_add_third_party("glog-0.7.0")
x_tools_add_third_party("qmdnsengine-master")
x_tools_add_third_party("libqrencode-master")
x_tools_git_get_latest_commit(${CMAKE_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_SOURCE_DIR} "X_TOOLS")
# --------------------------------------------------------------------------------------------------
# Common module
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/Common)
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/CommonUI)
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
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/QRCode/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/Broadcast/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/FileCheck/Source)
include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants/FileMerge/Source)

# --------------------------------------------------------------------------------------------------
# xTools application
if(EXISTS ${CMAKE_SOURCE_DIR}/Private/xTools/xToolsPrivate.cmake)
  include(${CMAKE_SOURCE_DIR}/Private/xTools/xToolsPrivate.cmake)
endif()
x_tools_add_executable(xTools ${X_TOOLS_SOURCE})
x_tools_generate_translations(xTools)
x_tools_deploy_qt(xTools)

target_link_libraries(xTools PRIVATE qrencode)
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
if(X_TOOLS_PRIVATE_MODULE_IS_VALID)
  target_link_libraries(xTools PRIVATE QtAES::QtAES)
endif()

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
