# cmake-format: off
cmake_minimum_required(VERSION 3.22)
set(CMAKE_POLICY_VERSION_MINIMUM "3.22" CACHE STRING "CMake policy version" FORCE)
set(QT_NO_PRIVATE_MODULE_WARNING CACHE BOOL "Suppress Qt private module warnings" ON)
# cmake-format: on

# --------------------------------------------------------------------------------------------------
# * Using vcpkg, you must set the environment variable VCPKG_ROOT first
# * You can set VCPKG_TARGET_TRIPLET value to specify the vcpkg triplet, such as x64-windows,
#   x64-windows-static, etc.
option(X_USING_VCPKG "Using vcpkg" OFF)
if(X_USING_VCPKG)
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Please specify your own vcpkg.cmake file")
endif()

# --------------------------------------------------------------------------------------------------
# Project settings
project(
  xTools
  VERSION 7.0
  LANGUAGES C CXX
  DESCRIPTION "xTools - All in one debugging and development tools")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# cmake-format: off
set(WITH_GFLAGS OFF)
set(BUILD_TESTING OFF)
set(WITH_TOOLS OFF CACHE BOOL "Reset option" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Reset option" FORCE)
# cmake-format: on

# --------------------------------------------------------------------------------------------------
# CMake module
include(${CMAKE_SOURCE_DIR}/cmake/x.cmake)
x_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X")
x_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X")
x_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X")
if(NOT DEFINED X_LATEST_GIT_TAG)
  set(X_LATEST_GIT_TAG "9.9.9")
endif()
if(X_LATEST_GIT_TAG MATCHES "^v[0-9].*")
  # Remove "v" prefix if exists
  string(SUBSTRING ${X_LATEST_GIT_TAG} 1 -1 X_LATEST_GIT_TAG)
endif()

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

if(CMAKE_CXX_COMPILER_VERSION AND MSVC)
  set(compiler_version ${CMAKE_CXX_COMPILER_VERSION})
  if(compiler_version VERSION_GREATER_EQUAL "19.50")
    message(STATUS "[xTools] MSVC_VERSION: msvc2026")
  elseif(compiler_version VERSION_GREATER_EQUAL "19.30")
    message(STATUS "[xTools] MSVC_VERSION: msvc2022")
  elseif(compiler_version VERSION_GREATER_EQUAL "19.20")
    message(STATUS "[xTools] MSVC_VERSION: msvc2019")
  elseif(compiler_version VERSION_GREATER_EQUAL "19.10")
    message(STATUS "[xTools] MSVC_VERSION: msvc2017")
  elseif(compiler_version VERSION_GREATER_EQUAL "19.00")
    message(STATUS "[xTools] MSVC_VERSION: msvc2015")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Qt6 is preferred, but Qt5 is also supported
find_package(QT NAMES Qt5 Qt6 REQUIRED)
# Qt modules
list(APPEND X_QT_COMPONENTS Gui)
list(APPEND X_QT_COMPONENTS Svg)
list(APPEND X_QT_COMPONENTS Qml)
list(APPEND X_QT_COMPONENTS Core)
list(APPEND X_QT_COMPONENTS Widgets)
list(APPEND X_QT_COMPONENTS Network)
list(APPEND X_QT_COMPONENTS LinguistTools)
if(QT_VERSION VERSION_GREATER_EQUAL "6.10.0")
  list(APPEND X_QT_COMPONENTS CorePrivate)
  list(APPEND X_QT_COMPONENTS GuiPrivate)
endif()
# Find Qt modules
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${X_QT_COMPONENTS})
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Gui)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Svg)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Qml)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Core)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Network)
list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
if(QT_VERSION VERSION_LESS "5.9.0")
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if(QT_VERSION VERSION_LESS "5.12.0")
  include_directories(${QT_DIR}/../../../include/QtCore/${QT_VERSION})
  include_directories(${QT_DIR}/../../../include/QtCore/${QT_VERSION}/QtCore)
  include_directories(${QT_DIR}/../../../include/QtGui/${QT_VERSION})
  include_directories(${QT_DIR}/../../../include/QtGui/${QT_VERSION}/QtGui)
endif()
if(QT_VERSION VERSION_GREATER_EQUAL "6.10.0")
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::CorePrivate)
  list(APPEND X_LIBS Qt${QT_VERSION_MAJOR}::GuiPrivate)
endif()

# --------------------------------------------------------------------------------------------------
# Global settings
set(X_BINS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
set(X_3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
set(tmp ${CMAKE_SOURCE_DIR}/libs/${QT_VERSION})
set(X_LIBS_DIR ${tmp}/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ID}/${CMAKE_CXX_COMPILER_VERSION})
message(STATUS "[xTools] X_LIBS_DIR: ${X_LIBS_DIR}")
message(STATUS "------------------------------------------------------------")

# Do not change X_MAGIC option unless you know what you are doing
option(X_MAGIC "The magic option..." OFF)
if(X_MAGIC)
  add_compile_definitions(X_MAGIC)
endif()

# Do not change X_LATEST_GIT_TAG unless you know what you are doing
set(X_ASSET_NAME "xTools-${CMAKE_SYSTEM_NAME}-v${X_LATEST_GIT_TAG}-${CMAKE_SYSTEM_PROCESSOR}")
if(${QT_VERSION} VERSION_LESS "6.0.0" AND WIN32)
  set(X_ASSET_NAME "${X_ASSET_NAME}-win7")
endif()
string(TOLOWER ${X_ASSET_NAME} X_ASSET_NAME)
message(STATUS "[xTools] Asset name: ${X_ASSET_NAME}")

# --------------------------------------------------------------------------------------------------
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
# QT_TARGET_TYPE is SHARED_LIBRARY or STATIC_LIBRARY
get_target_property(QT_TARGET_TYPE Qt${QT_VERSION_MAJOR}::Core TYPE)
if(WIN32 AND MSVC)
  option(X_STATIC_VC_LIBS "Do not change the option unless you know what you are doing" OFF)
  if(QT_TARGET_TYPE STREQUAL "STATIC_LIBRARY" AND ${X_STATIC_VC_LIBS})
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Get all source files of the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE X_SOURCES_H "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE X_SOURCES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE X_SOURCES_UI "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui")
set(X_SOURCES ${X_SOURCES_H} ${X_SOURCES_CPP} ${X_SOURCES_UI})
list(APPEND X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.qrc)
list(APPEND X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.rc)

# GPS module
option(X_ENABLE_GPS "Enable GPS Test device" OFF)
if(X_ENABLE_GPS)
  add_compile_definitions(X_ENABLE_GPS=1)
else()
  add_compile_definitions(X_ENABLE_GPS=0)
endif()

# TFTP module
option(X_ENABLE_TFTP "Enable TFTP Module" OFF)
if(X_ENABLE_TFTP)
  add_compile_definitions(X_ENABLE_TFTP=1)
else()
  add_compile_definitions(X_ENABLE_TFTP=0)
  file(GLOB_RECURSE X_SOURCES_TFTP "src/tools/tftp/*.*")
  foreach(source ${X_SOURCES_TFTP})
    list(REMOVE_ITEM X_SOURCES ${source})
    message(STATUS "[xTools.tftp] Remove TFTP source: ${source}")
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# xApplications selector(just for Qt6.8.0 or later)
include(${CMAKE_SOURCE_DIR}/xapps/xApp.cmake)
if(NOT ${X_APP} STREQUAL "xTools")
  return()
endif()

# --------------------------------------------------------------------------------------------------
# Setup 3rd party libraries and Qt modules
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt.cmake)
include(FindPkgConfig)

# Do something for Qt5(vc++)
if(QT_VERSION_MAJOR EQUAL 5 AND MSVC)
  if(NOT X_ENABLE_X_FLOW)
    add_compile_options(/execution-charset:utf-8)
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
set(bin ${X_BINS_DIR}/xTools)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin})
x_add_executable(xTools ${X_SOURCES})
x_output_env(xTools)
x_deploy_qt(xTools)
x_deploy_resources(xTools)
x_generate_translations(xTools)
target_link_libraries(xTools PRIVATE ${X_LIBS})
get_target_property(X_LIBS_LINKED_LIBS xTools LINK_LIBRARIES)
message(STATUS "[xTools] xTools linked libs: ${X_LIBS_LINKED_LIBS}")
get_target_property(XTOOLS_RUNTIME_OUTPUT_DIR xTools RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "[xTools] xTools runtime output directory: ${XTOOLS_RUNTIME_OUTPUT_DIR}")

# --------------------------------------------------------------------------------------------------
# Windows
if(WIN32)
  target_link_libraries(xTools PRIVATE Dwmapi Ws2_32)
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
  if(NOT QT_VERSION VERSION_LESS "6.8.0")
    set(x_android_source ${CMAKE_SOURCE_DIR}/res/android/6.8)
    set_target_properties(xTools PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${x_android_source})
  endif()
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
  # Set the icon file for the macOS bundle
  set(MACOSX_BUNDLE_ICON_FILE xTools.icns)
  set(xTools_ICON xTools.icns)
  set_source_files_properties(${xTools_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  target_sources(xTools PRIVATE ${xTools_ICON})

  # Set the resource files for the macOS bundle
  set(tmp "Resources/Assets.xcassets")
  set(RESOURCE_FILES "${CMAKE_SOURCE_DIR}/res/apple/macos/Assets.xcassets")
  set_source_files_properties(${RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION ${tmp})
  target_sources(xTools PRIVATE ${RESOURCE_FILES})
  set_target_properties(xTools PROPERTIES MACOSX_BUNDLE TRUE)

  # Create dmg
  include(${CMAKE_SOURCE_DIR}/cmake/dmg/dmg.cmake)
  x_generate_dmg(xTools ${X_LATEST_GIT_TAG})

  if(CMAKE_GENERATOR STREQUAL "Xcode")
    # Configure the Info.plist file for the macOS bundle
    string(TIMESTAMP X_CURRENT_YEAR "%Y")
    message(STATUS "Current year: ${X_CURRENT_YEAR}")
    string(TIMESTAMP X_VERSION "${X_LATEST_GIT_TAG}")
    string(TIMESTAMP X_BUNDLE_VERSION "%Y%m%d%H%M")
    message(STATUS "[xTools] X_BUNDLE_VERSION: ${X_BUNDLE_VERSION}")
    set(X_PLIST "${CMAKE_BINARY_DIR}/xTools.plist")
    set(X_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/res/apple/macos/xTools.entitlements")
    configure_file(res/apple/macos/xTools.plist.in ${X_PLIST})

    set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${X_PLIST})
    set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "com.xtools.xtools")
    if(DEFINED BUNDLE_ID_OPTION)
      set_target_properties(xTools PROPERTIES ${BUNDLE_ID_OPTION})
    endif()

    set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "8U23DJG99F")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic")
    set(CMAKE_XCODE_ATTRIBUTE_INSTALL_PATH "/Applications")
    # * set(CMAKE_XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${XTOOLS_APP_ID}")
    # * set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT
    # * set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS
    # * set(CMAKE_XCODE_ATTRIBUTE_LD_ENTRY_POINT
    # * set(CMAKE_XCODE_ATTRIBUTE_MARKETING_VERSION
    # * set(CMAKE_XCODE_ATTRIBUTE_CURRENT_PROJECT_VERSION
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${X_ENTITLEMENTS}")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Otiose(Just for test)
option(X_ENABLE_OTIOSE "Do not change the option unless you know what you are doing" OFF)
if(X_ENABLE_OTIOSE)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/otiose)
endif()

# --------------------------------------------------------------------------------------------------
# Do something after configuration
option(X_COPY_COMPILE_COMMANDS "Copy compile_commands.json to source directory" OFF)
if(X_COPY_COMPILE_COMMANDS)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
