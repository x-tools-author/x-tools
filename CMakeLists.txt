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

set(X_TOOLS_QT_COMPONENTS Core Gui Widgets WebSockets Network)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
  list(APPEND X_TOOLS_QT_COMPONENTS SerialPort)
  list(APPEND X_TOOLS_QT_COMPONENTS SerialBus)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_SERIALPORT)
  add_compile_definitions(X_TOOLS_IMPORT_MODULE_SERIALBUS)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})

include(CMake/sak_common.cmake)

file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/Common/*.h")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/Common/*.ui")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/Common/*.cc")

file(GLOB_RECURSE X_TOOLS_TOOLS_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/Tools/*.h")
file(GLOB_RECURSE X_TOOLS_TOOLS_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/Tools/*.ui")
file(GLOB_RECURSE X_TOOLS_TOOLS_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/Tools/*.cc")

file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/ToolBox/*.h")
file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/ToolBox/*.ui")
file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/ToolBox/*.cc")

file(GLOB X_TOOLS_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/*.h")
file(GLOB X_TOOLS_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/*.ui")
file(GLOB X_TOOLS_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/*.cc")

file(GLOB X_TOOLS_ASSISTANT_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/Assistants/*/src/*.h")
file(GLOB X_TOOLS_ASSISTANT_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/Assistants/*/src/*.ui")
file(GLOB X_TOOLS_ASSISTANT_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/Assistants/*/src/*.cc")

file(GLOB X_TOOLS_CMAKE_FILES "${CMAKE_SOURCE_DIR}/CMake/*.cmake")

list(APPEND X_TOOLS_SOURCE ${X_TOOLS_COMMOM_SOURCE_H})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_COMMOM_SOURCE_UI})
list(APPEND X_TOOLS_SOURCE ${X_TOOLS_COMMOM_SOURCE_CPP})
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

include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants)
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/Common)
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/CommonUI)
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/Tools)
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBox)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBoxUI)

set(WITH_GFLAGS OFF)
set(BUILD_SHARED_LIBS OFF)

sak_add_subdirectory("glog-0.7.0")
sak_add_subdirectory("Qt-AES-master")
sak_add_subdirectory("nodeeditor-3.0.10")
sak_add_subdirectory("qmdnsengine-master")

qt_add_executable(xTools ${X_TOOLS_SOURCE})
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::SerialBus)
endif()
target_link_libraries(xTools PRIVATE qmdnsengine)
target_link_libraries(xTools PRIVATE glog::glog)
target_link_libraries(xTools PRIVATE QtAES::QtAES)
target_link_libraries(xTools PRIVATE QtNodes::QtNodes)
