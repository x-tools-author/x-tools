# cmake-format: off
# --------------------------------------------------------------------------------------------------
# Do something make CMake happy
if(NOT ${CMAKE_VERSION} VERSION_LESS "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM "3.16" CACHE STRING "CMake policy version" FORCE)
endif()

cmake_minimum_required(VERSION 3.22)
project(xTools VERSION 8.0 LANGUAGES C CXX)
# cmake-format: on

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------------------------------------------------------
# Some options for 3rd party libraries
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
find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_TOOLS_QT_COMPONENTS})

# --------------------------------------------------------------------------------------------------
# Check Qt version
if(QT_VERSION VERSION_LESS "5.6.0")
  message(FATAL_ERROR "Qt 5.6.0 or later is required")
endif()

# Qt 6.8.0 or later is recommended, or output a warning message
if(QT_VERSION VERSION_LESS "6.8.0")
  message(WARNING "Qt 6.8.0 or later is recommended")
endif()

# --------------------------------------------------------------------------------------------------
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
# Get all source files of the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE X_TOOLS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*")
list(APPEND X_TOOLS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.qrc)

# --------------------------------------------------------------------------------------------------
# CMake module
include(${CMAKE_SOURCE_DIR}/cmake/git.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/xtools.cmake)
x_tools_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")

# --------------------------------------------------------------------------------------------------
# Target platform
message(STATUS "------------------------------------------------------------")
message(STATUS "[xTools]CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "[xTools]CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
message(STATUS "[xTools]CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "[xTools]CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "[xTools]CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "[xTools]CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "[xTools]CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "[xTools]CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "[xTools]CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "[xTools]CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "[xTools]CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

set(X_ASSET_NAME "xTools-${CMAKE_SYSTEM_NAME}-${X_TOOLS_LATEST_GIT_TAG}-${CMAKE_SYSTEM_PROCESSOR}")
string(TOLOWER ${X_ASSET_NAME} X_ASSET_NAME)
message(STATUS "[xTools]Asset name: ${X_ASSET_NAME}")

set(libs_dir1 ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(libs_dir2 ${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/${QT_VERSION}/${CMAKE_CXX_COMPILER_ID})
set(X_DEPLOY_LIBS_DIR ${libs_dir1}/${libs_dir2})
message(STATUS "[xTools]Deploy libraries directory: ${X_DEPLOY_LIBS_DIR}")
message(STATUS "------------------------------------------------------------")

# --------------------------------------------------------------------------------------------------
# 3rd module
include(${CMAKE_SOURCE_DIR}/cmake/glog.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/QXlsx.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/qmdnsengine.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/libqrencode.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Qt-Advanced-Stylesheets.cmake)

# --------------------------------------------------------------------------------------------------
# The magic option...
option(X_TOOLS_MAGIC "The magic option..." OFF)
if(X_TOOLS_MAGIC)
  add_compile_definitions(X_TOOLS_MAGIC)
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialPort module
option(X_ENABLE_SERIAL_PORT "Enable SerialPort module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialPort)
if(NOT Qt${QT_VERSION_MAJOR}SerialPort_FOUND)
  set(X_ENABLE_SERIAL_PORT OFF)
endif()
if(X_ENABLE_SERIAL_PORT)
  add_compile_definitions(X_ENABLE_SERIAL_PORT)
else()
  message(STATUS "SerialPort module is disable, SerialPort files will be removed.")
  file(GLOB_RECURSE SERIAL_PORT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/serialport*")
  foreach(file ${SERIAL_PORT_FILES})
    list(REMOVE_ITEM X_TOOLS_SOURCES ${file})
    message(STATUS "[SerialPort]Remove file: ${file}")
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# Qt WebSockets module
option(X_ENABLE_WEB_SOCKET "Enable WebSockets module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS WebSockets)
if(NOT Qt${QT_VERSION_MAJOR}WebSockets_FOUND)
  set(X_ENABLE_WEB_SOCKET OFF)
endif()
if(X_ENABLE_WEB_SOCKET)
  add_compile_definitions(X_ENABLE_WEB_SOCKET)
else()
  message(STATUS "WebSockets module is disable, WebSockets files will be removed.")

  file(GLOB_RECURSE SERIAL_PORT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/websocket*")
  foreach(file ${SERIAL_PORT_FILES})
    list(REMOVE_ITEM X_TOOLS_SOURCES ${file})
    message(STATUS "[WebSockets]Remove file: ${file}")
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# Qt Bluetooth module
option(X_ENABLE_BLUETOOTH "Enable Bluetooth module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Bluetooth)
if((NOT Qt${QT_VERSION_MAJOR}Bluetooth_FOUND) OR (QT_VERSION VERSION_LESS 6.5.0))
  set(X_ENABLE_BLUETOOTH OFF)
endif()
if(X_ENABLE_BLUETOOTH)
  add_compile_definitions(X_ENABLE_BLUETOOTH)
else()
  message(STATUS "Bluetooth module is disable, Bluetooth files will be removed.")
  file(GLOB_RECURSE SERIAL_PORT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/ble*")
  foreach(file ${SERIAL_PORT_FILES})
    list(REMOVE_ITEM X_TOOLS_SOURCES ${file})
    message(STATUS "[Bluetooth]Remove file: ${file}")
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialBus module
option(X_TOOLS_ENABLE_SERIALBUS "Enable SerialBus module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialBus)
if(Qt${QT_VERSION_MAJOR}SerialBus_FOUND)
  add_compile_definitions(X_TOOLS_ENABLE_SERIALBUS)
else()
  set(X_TOOLS_ENABLE_SERIALBUS OFF)

  # Remove modbus files
  file(GLOB_RECURSE MODBUS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tools/modbus/*.*")
  foreach(file ${MODBUS_SOURCE})
    list(REMOVE_ITEM X_TOOLS_SOURCES ${file})
  endforeach(file ${MODBUS_SOURCE})

  # Remove canbus files
  file(GLOB_RECURSE CANBUS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tools/canbus/*.*")
  foreach(file ${CANBUS_SOURCE})
    list(REMOVE_ITEM X_TOOLS_SOURCES ${file})
  endforeach(file ${CANBUS_SOURCE})
endif()

# --------------------------------------------------------------------------------------------------
# Qt Charts module
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Charts)
if(Qt${QT_VERSION_MAJOR}Charts_FOUND)
  option(X_ENABLE_CHARTS "Enable Charts module" ON)
  add_compile_definitions(X_ENABLE_CHARTS)
else()
  message(STATUS "Charts module is disable, chart files will be removed.")
  file(GLOB_RECURSE CHARTS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/page/charts/*.*")
  foreach(file ${CHARTS_SOURCE})
    message(STATUS "[Charts]Remove file: ${file}")
    list(REMOVE_ITEM X_TOOLS_SOURCES ${file})
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
set(bin ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/xTools)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin})
if(APPLE)
  set(MACOSX_BUNDLE_ICON_FILE xTools.icns)
  set(xTools_ICON ${CMAKE_SOURCE_DIR}/xTools.icns)
  set_source_files_properties(${xTools_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  list(APPEND X_TOOLS_SOURCES ${xTools_ICON})
endif()

if(WIN32)
  list(APPEND X_TOOLS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.rc)
endif()

x_tools_add_executable(xTools ${X_TOOLS_SOURCES})
x_tools_generate_translations(xTools)
x_tools_output_env(xTools)
x_tools_deploy_qt(xTools)
x_tools_deploy_qss(xTools)
x_tools_install_glog(xTools)
x_tools_install_qxlsx(xTools)
x_tools_install_qmdns(xTools)
x_tools_install_qrencode(xTools)

set(X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::CorePrivate)
list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::CorePrivate)
list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::Core)
list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::Gui)
list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::Network)
list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::Svg)
list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
list(APPEND X_TOOLS_LIBS ${x_qrencode} glog::glog qmdnsengine)
if(X_ENABLE_SERIAL_PORT)
  list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::SerialPort)
endif()
if(X_ENABLE_WEB_SOCKET)
  list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::WebSockets)
endif()
if(X_TOOLS_ENABLE_SERIALBUS)
  list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::SerialBus)
endif()
if(X_ENABLE_BLUETOOTH)
  list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()
if(X_ENABLE_CHARTS)
  list(APPEND X_TOOLS_LIBS Qt${QT_VERSION_MAJOR}::Charts QXlsx::QXlsx)
endif()
if(X_TOOLS_ENABLE_QSS)
  list(APPEND X_TOOLS_LIBS QssMgr)
endif()

target_link_libraries(xTools PRIVATE ${X_TOOLS_LIBS})

# --------------------------------------------------------------------------------------------------
# if X_TOOLS_LATEST_GIT_TAG contain "v", remove it...
if(NOT X_TOOLS_LATEST_GIT_TAG)
  set(X_TOOLS_LATEST_GIT_TAG "9.9.9")
endif()

if(X_TOOLS_LATEST_GIT_TAG MATCHES "^v[0-9].*")
  string(SUBSTRING ${X_TOOLS_LATEST_GIT_TAG} 1 -1 X_TOOLS_LATEST_GIT_TAG)
endif()

# --------------------------------------------------------------------------------------------------
# Make installer for Windows
if(WIN32 AND X_TOOLS_LATEST_GIT_TAG)
  include(${CMAKE_SOURCE_DIR}/cmake/msix/msix.cmake)
  include(${CMAKE_SOURCE_DIR}/cmake/qifw/qifw.cmake)
  set(icon ${CMAKE_CURRENT_SOURCE_DIR}/xTools.ico)
  x_tools_generate_zip(xTools ${X_TOOLS_LATEST_GIT_TAG})
  x_tools_generate_msix(xTools "QSAK" "xTools Pro" ${X_TOOLS_LATEST_GIT_TAG} FALSE)
  x_tools_generate_installer(xTools ${X_TOOLS_LATEST_GIT_TAG} ${icon})
endif()

# --------------------------------------------------------------------------------------------------
# Make package for linux(deb or appimage)
if(LINUX)
  include(cmake/linux/linux.cmake)
  x_tools_make_package(xTools "x-tools" "xTools" ${X_TOOLS_LATEST_GIT_TAG} FALSE)
  x_tools_make_package(xTools "x-tools" "xTools" ${X_TOOLS_LATEST_GIT_TAG} TRUE)
endif()

# --------------------------------------------------------------------------------------------------
# Make dmg for macOS
if(APPLE)
  include(${CMAKE_SOURCE_DIR}/cmake/dmg/dmg.cmake)
  x_tools_generate_dmg(xTools ${X_TOOLS_LATEST_GIT_TAG})
endif()

# --------------------------------------------------------------------------------------------------
# ???
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(xTools)
endif()

# --------------------------------------------------------------------------------------------------
# Some apps base xTools
include(${CMAKE_SOURCE_DIR}/xapps/xapps.cmake)
