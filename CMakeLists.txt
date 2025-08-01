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
  # set(VCPKG_TARGET_TRIPLET "x64-windows-static")
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Please specify your own vcpkg file")
  if (${VCPKG_TARGET_TRIPLET} STREQUAL "x64-windows-static")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
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
include(${CMAKE_SOURCE_DIR}/cmake/x.cmake)
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
find_package(QT NAMES Qt5 Qt6 REQUIRED)
if(QT_VERSION VERSION_GREATER_EQUAL "6.10.0")
  list(APPEND X_QT_COMPONENTS CorePrivate)
  list(APPEND X_QT_COMPONENTS GuiPrivate)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_QT_COMPONENTS})
set(X_LIBS Qt${QT_VERSION_MAJOR}::Core)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Gui)
if(QT_VERSION VERSION_GREATER_EQUAL "6.10.0")
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::CorePrivate)
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::GuiPrivate)
endif()
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Network)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Svg)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

# --------------------------------------------------------------------------------------------------
# Binaries
set(X_BINS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

# Libraries
set(tmp ${CMAKE_CURRENT_SOURCE_DIR}/libs/${QT_VERSION}/${CMAKE_BUILD_TYPE})
set(X_LIBS_DIR ${tmp}/${CMAKE_CXX_COMPILER_ID}/${CMAKE_CXX_COMPILER_VERSION})
message(STATUS "[xTools]X_LIBS_DIR: ${X_LIBS_DIR}")
message(STATUS "------------------------------------------------------------")

# Do not change X_LATEST_GIT_TAG unless you know what you are doing
option(X_MAGIC "The magic option..." OFF)
if(X_MAGIC)
  add_compile_definitions(X_MAGIC)
endif()

# Do not change X_ENABLE_LUA option unless you know what you are doing
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

include(${CMAKE_SOURCE_DIR}/cmake/x_3rd.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt.cmake)
message(STATUS "[xTools]Link libraries: ${X_LIBS}")

# --------------------------------------------------------------------------------------------------
# xApplications selector(just for Qt6.8.0 or later)
include(${CMAKE_SOURCE_DIR}/xapps/xApp.cmake)
if(NOT ${X_APP} STREQUAL "xTools")
  return()
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
set(bin ${X_BINS_DIR}/xTools)
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
