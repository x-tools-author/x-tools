# cmake-format: off
# --------------------------------------------------------------------------------------------------
# Do something make CMake happy
if(NOT ${CMAKE_VERSION} VERSION_LESS "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM "3.22" CACHE STRING "CMake policy version" FORCE)
endif()

# --------------------------------------------------------------------------------------------------
# Using vcpkg(The 'x64-windows-static' triplet is supported only)
option(X_VCPKG "Using vcpkg" OFF)
if(X_VCPKG)
set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Please specify your own vcpkg file" FORCE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

cmake_minimum_required(VERSION 3.22)
project(xTools VERSION 8.0 LANGUAGES C CXX)
# cmake-format: on

set(CMAKE_CXX_STANDARD 17)
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
# CMake module
include(${CMAKE_SOURCE_DIR}/cmake/git.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/xtools.cmake)
x_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X")
x_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X")
x_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X")

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

# --------------------------------------------------------------------------------------------------
# Global variables
set(X_BIN ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
option(X_MAGIC "The magic option..." OFF)
if(X_MAGIC)
  add_compile_definitions(X_MAGIC)
endif()

# --------------------------------------------------------------------------------------------------
# Qt module
list(APPEND X_QT_COMPONENTS Gui)
list(APPEND X_QT_COMPONENTS Svg)
list(APPEND X_QT_COMPONENTS Core)
list(APPEND X_QT_COMPONENTS Widgets)
list(APPEND X_QT_COMPONENTS Network)
list(APPEND X_QT_COMPONENTS LinguistTools)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt6 is preferred, but Qt5 is also supported
find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS ${X_QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_QT_COMPONENTS})

set(libs_dir1 ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(libs_dir2 ${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/${QT_VERSION}/${CMAKE_CXX_COMPILER_ID})
set(X_DEPLOY_LIBS_DIR ${libs_dir1}/${libs_dir2})
message(STATUS "[xTools]Deploy libraries directory: ${X_DEPLOY_LIBS_DIR}")
message(STATUS "------------------------------------------------------------")

set(X_ASSET_NAME "xTools-${CMAKE_SYSTEM_NAME}-${X_LATEST_GIT_TAG}-${CMAKE_SYSTEM_PROCESSOR}")
if(${QT_VERSION} VERSION_LESS "6.0.0" AND WIN32)
  set(X_ASSET_NAME "${X_ASSET_NAME}-win7")
endif()
string(TOLOWER ${X_ASSET_NAME} X_ASSET_NAME)
message(STATUS "[xTools]Asset name: ${X_ASSET_NAME}")

# --------------------------------------------------------------------------------------------------
# Do something for Apple platforms
if(IOS)
  message(FATAL_ERROR "xTools does not support iOS platform yet...")
endif()

# Set macOS deployment target (minimum macOS version)
if(APPLE AND NOT IOS)
  # Set minimum macOS version - you can adjust this as needed Common options: 11.0 (Big Sur), 12.0
  # (Monterey), 13.0 (Ventura), 14.0 (Sonoma)
  if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET
        "11.0"
        CACHE STRING "Minimum macOS deployment version")
  endif()
  message(STATUS "[xTools]macOS deployment target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
endif()

# Do something for Qt5
if(APPLE AND NOT IOS)
  if(QT_VERSION VERSION_LESS "6.0.0")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Check Qt version
if(QT_VERSION VERSION_LESS "5.9.0")
  message(FATAL_ERROR "Qt 5.9.0 or later is required")
endif()

# Qt 6.8.0 or later is recommended, or output a warning message
if(QT_VERSION VERSION_LESS "6.8.0")
  message(WARNING "Qt 6.8.0 or later is recommended")
  if(ANDROID)
    message(FATAL_ERROR "Qt 6.8.0 or later is required for Android platform")
  endif()
endif()

if(QT_VERSION_MAJOR EQUAL 5)
  if(MSVC)
    add_compile_options(/execution-charset:utf-8)
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Don't change X_STATIC_VC_RUNTIME_LIBRARY option unless you know what you are doing. QT_TARGET_TYPE
# is SHARED_LIBRARY or STATIC_LIBRARY
get_target_property(QT_TARGET_TYPE Qt${QT_VERSION_MAJOR}::Core TYPE)
option(X_STATIC_VC_RUNTIME_LIBRARY "Using static vc runtime library" OFF)
if(WIN32 AND MSVC)
  if(QT_TARGET_TYPE STREQUAL "STATIC_LIBRARY" AND X_STATIC_VC_RUNTIME_LIBRARY)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Get all source files of the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE X_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*")
list(APPEND X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.qrc)

# --------------------------------------------------------------------------------------------------
# 3rd module
include(${CMAKE_SOURCE_DIR}/cmake/glog.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/qxlsx.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/qmdnsengine.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/libqrencode.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/libiconv.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/qcustomplot.cmake)

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
    list(REMOVE_ITEM X_SOURCES ${file})
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
    list(REMOVE_ITEM X_SOURCES ${file})
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
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[Bluetooth]Remove file: ${file}")
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialBus module
option(X_ENABLE_SERIALBUS "Enable SerialBus module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialBus)
if(Qt${QT_VERSION_MAJOR}SerialBus_FOUND)
  add_compile_definitions(X_ENABLE_SERIALBUS)
else()
  set(X_ENABLE_SERIALBUS OFF)

  # Remove modbus files
  file(GLOB_RECURSE MODBUS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tools/modbus/*.*")
  foreach(file ${MODBUS_SOURCE})
    list(REMOVE_ITEM X_SOURCES ${file})
  endforeach(file ${MODBUS_SOURCE})

  # Remove canbus files
  file(GLOB_RECURSE CANBUS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tools/canbus/*.*")
  foreach(file ${CANBUS_SOURCE})
    list(REMOVE_ITEM X_SOURCES ${file})
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
    list(REMOVE_ITEM X_SOURCES ${file})
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# Libs
set(X_LIBS Qt${QT_VERSION_MAJOR}::CorePrivate)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::CorePrivate)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Core)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Gui)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Network)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Svg)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
list(APPEND X_LIBS ${x_qrencode} glog::glog qmdnsengine)
if(X_ENABLE_SERIAL_PORT)
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::SerialPort)
endif()
if(X_ENABLE_WEB_SOCKET)
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::WebSockets)
endif()
if(X_ENABLE_SERIALBUS)
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::SerialBus)
endif()
if(X_ENABLE_BLUETOOTH)
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()
if(X_ENABLE_CHARTS)
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Charts QXlsx::QXlsx)
endif()
if(X_ICONV)
  list(APPEND X_LIBS ${X_ICONV})
endif()
if(X_PLOT)
  list(APPEND X_LIBS QCustomPlot)
endif()

# --------------------------------------------------------------------------------------------------
# xApplications selector(just for Qt6.8.0 or later)
set(X_APPS_ENABLED
    OFF
    CACHE BOOL "Enable x-apps selector" FORCE)
if(NOT QT_VERSION VERSION_LESS "6.8.0" AND X_APPS_ENABLED)
  set(X_APPS "xTools")
  list(APPEND X_APPS "xSvg")
  list(APPEND X_APPS "xCode")
  list(APPEND X_APPS "xDemo")
  list(APPEND X_APPS "xHash")
  list(APPEND X_APPS "xPing")
  list(APPEND X_APPS "xDebug")
  list(APPEND X_APPS "xAssistant")
  list(APPEND X_APPS "xOscilloscope")

  if(EXISTS ${CMAKE_SOURCE_DIR}/xapps/xprivate)
    # Get all directories in xapps/xprivate
    file(GLOB PRIVATE_APPS "${CMAKE_SOURCE_DIR}/xapps/xprivate/*")
    foreach(private_app ${PRIVATE_APPS})
      if(IS_DIRECTORY ${private_app})
        get_filename_component(app_name ${private_app} NAME)
        # * The second char of the app name should be uppercase
        # * if app_name == xlinguist, the char2 is 'l', it will be converted to 'L'
        # * if app_name == xlinguist, the rest_of_name is 'inguist'
        # * if app_name == xlinguist, the app_name will be 'xLinguist'(x + L + inguist)
        string(SUBSTRING ${app_name} 1 1 char2)
        string(TOUPPER ${char2} char2)
        string(SUBSTRING "${app_name}" 2 -1 rest_of_name)
        string(CONCAT app_name "x${char2}${rest_of_name}")
        list(APPEND X_APPS "${app_name}")
      endif()
    endforeach()
  endif()

  # cmake-format: off
  set(X_APP "xTools" CACHE STRING "Select a x-app to build")
  set_property(CACHE X_APP PROPERTY STRINGS ${X_APPS})
  string(TOLOWER ${X_APP} LOWER_X_APP)
  if(EXISTS ${CMAKE_SOURCE_DIR}/xapps/${LOWER_X_APP})
    include_directories(${CMAKE_SOURCE_DIR}/xapps)
    include(${CMAKE_SOURCE_DIR}/xapps/${LOWER_X_APP}/${X_APP}.cmake)
    return()
  elseif(EXISTS ${CMAKE_SOURCE_DIR}/xapps/xprivate/${LOWER_X_APP})
    include_directories(${CMAKE_SOURCE_DIR}/xapps)
    include(${CMAKE_SOURCE_DIR}/xapps/xprivate/${LOWER_X_APP}/${X_APP}.cmake)
    return()
  endif()
  # cmake-format: on
endif()

# --------------------------------------------------------------------------------------------------
# if X_LATEST_GIT_TAG contain "v", remove it...
if(NOT X_LATEST_GIT_TAG)
  set(X_LATEST_GIT_TAG "9.9.9")
endif()

if(X_LATEST_GIT_TAG MATCHES "^v[0-9].*")
  string(SUBSTRING ${X_LATEST_GIT_TAG} 1 -1 X_LATEST_GIT_TAG)
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
set(bin ${X_BIN}/xTools)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin})

# Apple platforms settings
if(APPLE)
  set(MACOSX_BUNDLE_ICON_FILE xTools.icns)
  set(xTools_ICON ${CMAKE_SOURCE_DIR}/xTools.icns)
  set_source_files_properties(${xTools_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  list(APPEND X_SOURCES ${xTools_ICON})
endif()

if(WIN32)
  list(APPEND X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.rc)
endif()

x_add_executable(xTools ${X_SOURCES})
target_link_libraries(xTools PRIVATE ${X_LIBS})

x_generate_translations(xTools)
x_output_env(xTools)
x_deploy_qt(xTools)
x_install_qxlsx(xTools)
x_install_qmdns(xTools)
x_install_qrencode(xTools)

if(WIN32 AND MSVC)
  target_link_libraries(xTools PRIVATE Dwmapi)
elseif(LINUX)
  target_link_libraries(xTools PRIVATE dl)
endif()

# Android specific settings
if(ANDROID)
  set(x_android_source ${CMAKE_SOURCE_DIR}/res/android/6.8)
  target_link_libraries(xTools PRIVATE ${X_LIBS})
  set_target_properties(xTools PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${x_android_source})
endif()

# MacOS specific settings
if(APPLE AND NOT IOS)
  set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "com.xtools.xtools")
endif()

# --------------------------------------------------------------------------------------------------
# Make installer for Windows
if(WIN32 AND X_LATEST_GIT_TAG)
  include(${CMAKE_SOURCE_DIR}/cmake/msix/msix.cmake)
  include(${CMAKE_SOURCE_DIR}/cmake/qifw/qifw.cmake)
  set(icon ${CMAKE_CURRENT_SOURCE_DIR}/xTools.ico)
  x_generate_zip(xTools ${X_LATEST_GIT_TAG})
  x_generate_msix(xTools "QSAK" "xTools Pro" ${X_LATEST_GIT_TAG} FALSE)
  x_generate_installer(xTools ${X_LATEST_GIT_TAG} ${icon})
endif()

# --------------------------------------------------------------------------------------------------
# Make package for linux(deb or appimage)
if(LINUX)
  include(cmake/linux/linux.cmake)
  x_build_deb(xTools "x-tools" "xTools" ${X_LATEST_GIT_TAG} ${CMAKE_SOURCE_DIR}/xTools.png)
endif()

# --------------------------------------------------------------------------------------------------
# Make dmg for macOS
if(APPLE)
  include(${CMAKE_SOURCE_DIR}/cmake/dmg/dmg.cmake)
  x_generate_dmg(xTools ${X_LATEST_GIT_TAG})
endif()

# --------------------------------------------------------------------------------------------------
# ???
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_target(xTools)
endif()
