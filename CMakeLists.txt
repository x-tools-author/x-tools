# Do something for CMake 4.0.0 or later
if(NOT ${CMAKE_VERSION} VERSION_LESS "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM
      "3.22"
      CACHE STRING "CMake policy version" FORCE)
endif()

# Using vcpkg, you must set the environment variable VCPKG_ROOT first
option(X_USING_VCPKG "Using vcpkg" OFF)
if(X_USING_VCPKG)
  set(VCPKG_TARGET_TRIPLET
      "x64-windows-static"
      CACHE STRING "vcpkg target triplet")
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Please specify your own vcpkg file")
  if(${VCPKG_TARGET_TRIPLET} STREQUAL "x64-windows-static")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

cmake_minimum_required(VERSION 3.22)
project(
  xTools
  VERSION 8.0
  LANGUAGES C CXX
  DESCRIPTION "xTools - All in one toolbox for developers and engineers")

option(X_TOOLS_V8 "Build xTools version 8" OFF)
if(X_TOOLS_V8)
  set(CMAKE_CXX_STANDARD 20)
else()
  set(CMAKE_CXX_STANDARD 17)
endif()
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
message(STATUS "[xTools] CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "[xTools] CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
message(STATUS "[xTools] CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "[xTools] CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "[xTools] CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "[xTools] CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "[xTools] CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "[xTools] CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "[xTools] CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "[xTools] CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "[xTools] CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

# --------------------------------------------------------------------------------------------------
# Qt module
list(APPEND X_QT_COMPONENTS Gui)
list(APPEND X_QT_COMPONENTS Svg)
list(APPEND X_QT_COMPONENTS Qml)
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
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Qml)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

# --------------------------------------------------------------------------------------------------
# Global settings
set(X_BINS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
set(X_3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
set(tmp ${CMAKE_SOURCE_DIR}/libs)
set(X_LIBS_DIR ${tmp}/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ID}/${CMAKE_CXX_COMPILER_VERSION})
message(STATUS "[xTools] X_LIBS_DIR: ${X_LIBS_DIR}")
message(STATUS "------------------------------------------------------------")

# Do not change X_MAGIC option unless you know what you are doing
option(X_MAGIC "The magic option..." OFF)
if(X_MAGIC)
  add_compile_definitions(X_MAGIC)
endif()

# Do not change X_LATEST_GIT_TAG unless you know what you are doing
set(X_ASSET_NAME "xTools-${CMAKE_SYSTEM_NAME}-${X_LATEST_GIT_TAG}-${CMAKE_SYSTEM_PROCESSOR}")
if(${QT_VERSION} VERSION_LESS "6.0.0" AND WIN32)
  set(X_ASSET_NAME "${X_ASSET_NAME}-win7")
endif()
string(TOLOWER ${X_ASSET_NAME} X_ASSET_NAME)
message(STATUS "[xTools] Asset name: ${X_ASSET_NAME}")

# Set macOS deployment target (minimum macOS version)
if(APPLE AND NOT IOS)
  # Set minimum macOS version - you can adjust this as needed Common options: 12.0 (Monterey), 13.0
  # (Ventura), 14.0 (Sonoma), 15.0 (Sequoia), 26.0 (Tahoe)
  if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    # cmake-format: off
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum macOS deployment version")
    # cmake-format: on
  endif()

  if(QT_VERSION VERSION_LESS "6.0.0")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
  else()
    set(CMAKE_OSX_ARCHITECTURES "arm64")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Check Qt version
if(QT_VERSION VERSION_LESS "5.9.0")
  message(FATAL_ERROR "Qt 5.9.0 or later is required")
endif()

# Qt 6.8.0 or later is recommended, or output a warning message
if(QT_VERSION VERSION_LESS "6.8.0")
  if(ANDROID)
    message(FATAL_ERROR "Qt 6.8.0 or later is required for Android platform")
  else()
    message(WARNING "Qt 6.8.0 or later is recommended")
  endif()
endif()

# Do something for Qt5(vc++)
if(QT_VERSION_MAJOR EQUAL 5 AND MSVC)
  add_compile_options(/execution-charset:utf-8)
endif()

# --------------------------------------------------------------------------------------------------
# * Don't change X_STATIC_VC_RUNTIME_LIBRARY option unless you know what you are doing.
# * QT_TARGET_TYPE is SHARED_LIBRARY or STATIC_LIBRARY
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

# --------------------------------------------------------------------------------------------------
# if X_LATEST_GIT_TAG contain "v", remove it...
if(NOT DEFINED X_LATEST_GIT_TAG)
  set(X_LATEST_GIT_TAG "9.9.9")
endif()

if(X_LATEST_GIT_TAG MATCHES "^v[0-9].*")
  string(SUBSTRING ${X_LATEST_GIT_TAG} 1 -1 X_LATEST_GIT_TAG)
endif()

# --------------------------------------------------------------------------------------------------
# xApplications selector(just for Qt6.8.0 or later)
include(${CMAKE_SOURCE_DIR}/xapps/xApp.cmake)
if(NOT ${X_APP} STREQUAL "xTools")
  return()
endif()

# --------------------------------------------------------------------------------------------------
# xTools version 8 (Qt6.8.0 or later)
if(QT_VERSION VERSION_GREATER_EQUAL "6.8.0")
  if(X_TOOLS_V8)
    include(v8/xTools.cmake)
    return()
  endif()
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

# Inner xApps
include(${CMAKE_SOURCE_DIR}/cmake/x_apps.cmake)

# xTools
x_add_executable(xTools ${X_SOURCES})
target_link_libraries(xTools PRIVATE ${X_LIBS})
x_output_env(xTools)
x_deploy_qt(xTools)
x_deploy_resources(xTools)

# --------------------------------------------------------------------------------------------------
# xTools translations(i18n)
x_generate_module_translations(xtools ${CMAKE_CURRENT_SOURCE_DIR}/src OFF)

# Page modules
x_generate_module_translations(page ${CMAKE_CURRENT_SOURCE_DIR}/src/page OFF)
x_generate_module_translations(page-common ${CMAKE_CURRENT_SOURCE_DIR}/src/page/common ON)
x_generate_module_translations(page-emitter ${CMAKE_CURRENT_SOURCE_DIR}/src/page/emitter ON)
x_generate_module_translations(page-panels ${CMAKE_CURRENT_SOURCE_DIR}/src/page/panels ON)
x_generate_module_translations(page-preset ${CMAKE_CURRENT_SOURCE_DIR}/src/page/preset ON)
x_generate_module_translations(page-responder ${CMAKE_CURRENT_SOURCE_DIR}/src/page/responder ON)
x_generate_module_translations(page-transfer ${CMAKE_CURRENT_SOURCE_DIR}/src/page/transfer ON)
x_generate_module_translations(page-utilities ${CMAKE_CURRENT_SOURCE_DIR}/src/page/utilities ON)
x_generate_module_translations(page-scripts ${CMAKE_CURRENT_SOURCE_DIR}/src/page/scripts ON)

# Tools modules
x_generate_module_translations(tools ${CMAKE_CURRENT_SOURCE_DIR}/src/tools OFF)
x_generate_module_translations(tools-ascii ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/ascii ON)
x_generate_module_translations(tools-barcode ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/barcode ON)
x_generate_module_translations(tools-base64 ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/base64 ON)
x_generate_module_translations(tools-broadcast ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/broadcast ON)
x_generate_module_translations(tools-canbus ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/canbus ON)
x_generate_module_translations(tools-crc ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/crc ON)
x_generate_module_translations(tools-filecheck ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/filecheck ON)
x_generate_module_translations(tools-mdns ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/mdns ON)
x_generate_module_translations(tools-modbus ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/modbus ON)
x_generate_module_translations(tools-number ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/number ON)
x_generate_module_translations(tools-ping ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/ping ON)
x_generate_module_translations(tools-qrcode ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/qrcode ON)
x_generate_module_translations(tools-string ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/string ON)

# Other modules
x_generate_module_translations(common ${CMAKE_CURRENT_SOURCE_DIR}/src/common ON)
x_generate_module_translations(device ${CMAKE_CURRENT_SOURCE_DIR}/src/device ON)
x_generate_module_translations(nodeeditor ${CMAKE_CURRENT_SOURCE_DIR}/src/nodeeditor ON)

# --------------------------------------------------------------------------------------------------
# Windows
if(WIN32)
  if(MSVC)
    target_link_libraries(xTools PRIVATE Dwmapi)
  endif()

  include(${CMAKE_SOURCE_DIR}/cmake/msix/msix.cmake)
  include(${CMAKE_SOURCE_DIR}/cmake/qifw/qifw.cmake)
  set(icon ${CMAKE_CURRENT_SOURCE_DIR}/xTools.ico)
  x_generate_zip(xTools ${X_LATEST_GIT_TAG})
  x_generate_msix(xTools "QSAK" "xTools Pro" ${X_LATEST_GIT_TAG} FALSE)
  x_generate_installer(xTools ${X_LATEST_GIT_TAG} ${icon})
endif()

# --------------------------------------------------------------------------------------------------
# Android
if(ANDROID)
  set(x_android_source ${CMAKE_SOURCE_DIR}/res/android/6.8)
  set_target_properties(xTools PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${x_android_source})
endif()

# --------------------------------------------------------------------------------------------------
# Linux
if(LINUX AND NOT APPLE)
  target_link_libraries(xTools PRIVATE dl)
  include(cmake/linux/linux.cmake)
  x_build_deb(xTools "x-tools" "xTools" ${X_LATEST_GIT_TAG} ${CMAKE_SOURCE_DIR}/xTools.png)
endif()

# --------------------------------------------------------------------------------------------------
# Apple (macOS, iOS...)
if(APPLE)
  set_target_properties(
    xTools
    PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "xTools"
               MACOSX_BUNDLE_BUNDLE_VERSION "${X_LATEST_GIT_TAG}"
               MACOSX_BUNDLE_SHORT_VERSION_STRING "${X_LATEST_GIT_TAG}"
               MACOSX_BUNDLE_GUI_IDENTIFIER "com.xtools.xtools"
               XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
               XCODE_ATTRIBUTE_INFOPLIST_KEY_CFBundleIconFile "AppIcon"
               XCODE_ATTRIBUTE_INFOPLIST_KEY_CFBundleIconName "AppIcon"
               XCODE_ATTRIBUTE_INFOPLIST_KEY_CFBundleDisplayName "xTools"
               XCODE_ATTRIBUTE_INFOPLIST_KEY_CFBundleName "xTools"
               XCODE_ATTRIBUTE_PRODUCT_NAME "xTools")

  include(${CMAKE_SOURCE_DIR}/cmake/dmg/dmg.cmake)
  x_generate_dmg(xTools ${X_LATEST_GIT_TAG})

  include(${CMAKE_SOURCE_DIR}/cmake/pkg/pkg.cmake)
  set(XTOOLS_DEV_ID_APPLICATION
      "Developer ID Application: Your Name \(XXXXXXXXXX\)"
      CACHE STRING "Developer ID Application")
  set(XTOOLS_DEV_ID_INSTALLER
      "Developer ID Installer: Your Name \(XXXXXXXXXX\)"
      CACHE STRING "Developer ID Installer")
  x_generate_pkg(xTools ${X_LATEST_GIT_TAG} ${XTOOLS_DEV_ID_APPLICATION} ${XTOOLS_DEV_ID_INSTALLER})
endif()

# --------------------------------------------------------------------------------------------------
# otiose
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/otiose)

# --------------------------------------------------------------------------------------------------
# ......
get_target_property(X_LIBS_LINKED_LIBS xTools LINK_LIBRARIES)
message(STATUS "[xTools] xTools linked libs: ${X_LIBS_LINKED_LIBS}")
get_target_property(XTOOLS_RUNTIME_OUTPUT_DIR xTools RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "[xTools] xTools runtime output directory: ${XTOOLS_RUNTIME_OUTPUT_DIR}")

# --------------------------------------------------------------------------------------------------
# ???(I don't know why)
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_target(xTools)
endif()
