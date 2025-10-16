set(CMAKE_CXX_STANDARD 20)
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
if(QT_VERSION VERSION_LESS "6.8.0")
  message(FATAL_ERROR "Qt 6.8.0 or later is required")
endif()

# Qt 6.8.0 or later is recommended, or output a warning message
if(QT_VERSION VERSION_LESS "6.8.0")
  if(ANDROID)
    message(FATAL_ERROR "Qt 6.8.0 or later is required for Android platform")
  else()
    message(WARNING "Qt 6.8.0 or later is recommended")
  endif()
endif()

find_package(
  Qt6
  COMPONENTS Widgets Quick QuickControls2
  REQUIRED)

file(GLOB_RECURSE X_SOURCES "v8/src/*.*")
set(X_BIN "${CMAKE_BINARY_DIR}/xTools")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/xTools")
qt_add_executable(xTools ${X_SOURCES} xTools.qrc)
x_deploy_qt(xTools)
x_generate_translations(xTools)
target_link_libraries(xTools PRIVATE ${X_LIBS} Qt6::QuickControls2)
if(MSVC)
  target_link_libraries(xTools PRIVATE Dwmapi)
endif()

# --------------------------------------------------------------------------------------------------
# QML setup
file(GLOB_RECURSE qml_tmp_files "v8/qml/*.*")
foreach(qml_file ${qml_tmp_files})
  file(RELATIVE_PATH rel_path ${CMAKE_SOURCE_DIR} ${qml_file})
  list(APPEND qml_files ${rel_path})
endforeach()
qt_add_qml_module(
  xTools
  URI
  xToolsQml
  QML_FILES
  ${qml_files}
  OUTPUT_DIRECTORY
  ${X_BIN}/qml/)

set_target_properties(
  xTools
  PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER com.xtools.xtools
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

include(GNUInstallDirs)
install(
  TARGETS xTools
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
