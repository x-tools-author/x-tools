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

include(CMake/xToolsCommon.cmake)
include(CMake/xToolsGit.cmake)
include(CMake/xToolsDeployQt.cmake)
include(CMake/xToolsThirdParty.cmake)

file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_H "${CMAKE_SOURCE_DIR}/Source/Common/*.h")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_UI "${CMAKE_SOURCE_DIR}/Source/Common/*.ui")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_CPP "${CMAKE_SOURCE_DIR}/Source/Common/*.cpp")

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
list(APPEND X_TOOLS_SOURCE xTools.qrc)
if (WIN32)
    list(APPEND X_TOOLS_SOURCE xTools.rc)
endif()

include_directories(${CMAKE_SOURCE_DIR}/Source/Assistants)
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/Common)
include_directories(${CMAKE_SOURCE_DIR}/Source/Common/CommonUI)
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/Tools)
include_directories(${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBox)
include_directories(${CMAKE_SOURCE_DIR}/Source/ToolBox/ToolBoxUI)

set(WITH_GFLAGS OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF)

sak_add_subdirectory("glog-0.7.0")
sak_add_subdirectory("qmdnsengine-master")

qt_add_executable(xTools ${X_TOOLS_SOURCE})
sak_git_get_last_commit(${CMAKE_SOURCE_DIR} "SAK")
sak_git_get_last_commit_time(${CMAKE_SOURCE_DIR} "SAK")
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

# -------------------------------------------------------------------------------------------------
# Assistants
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Assistants)
