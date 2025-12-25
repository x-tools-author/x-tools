# --------------------------------------------------------------------------------------------------
# Find Qt5
find_package(QT NAMES Qt5 REQUIRED)
find_package(
  Qt5
  COMPONENTS SVG Widgets Network SerialPort WebSockets LinguistTools
  REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/execution-charset:utf-8)
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

# --------------------------------------------------------------------------------------------------
# Get all source files
file(GLOB_RECURSE X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/legacy/src/*.*)
list(APPEND tmp_dirs "common" "device" utilities)
foreach(tmp_dir ${tmp_dirs})
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/src/${tmp_dir})
  file(GLOB_RECURSE src_files ${dir}/*.*)
  list(APPEND X_SOURCES ${src_files})
endforeach()

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
# Remove useless files
list(FILTER X_SOURCES EXCLUDE REGEX ".*sctp.*")
list(FILTER X_SOURCES EXCLUDE REGEX "ble.*")
list(FILTER X_SOURCES EXCLUDE REGEX "charts.*")
list(FILTER X_SOURCES EXCLUDE REGEX "gps.*")
list(FILTER X_SOURCES EXCLUDE REGEX "hid.*")

# Qt include directories
include_directories(${QT_DIR}/../../../include/QtGui/${QT_VERSION})
include_directories(${QT_DIR}/../../../include/QtCore/${QT_VERSION})
include_directories(${QT_DIR}/../../../include/QtGui/${QT_VERSION}/QtGui)
include_directories(${QT_DIR}/../../../include/QtCore/${QT_VERSION}/QtCore)

# xTools executable
add_compile_definitions(X_DISABLE_LOG)
add_compile_definitions(X_ENABLE_SERIALPORT=1)
add_compile_definitions(X_ENABLE_GPS=0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(bin ${X_BINS_DIR}/xTools)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin})
add_executable(xTools ${X_SOURCES} xTools.qrc)
x_generate_zip(xTools ${X_LATEST_GIT_TAG})
set_target_properties(xTools PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(xTools PRIVATE Qt5::Svg Qt5::Widgets Qt5::Network Qt5::SerialPort
                                     Qt5::WebSockets)

# --------------------------------------------------------------------------------------------------
# Deploy Qt for Windows
if(NOT DEFINED WINDEPLOYQT_EXECUTABLE)
  set(WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
endif()
if(NOT EXISTS ${WINDEPLOYQT_EXECUTABLE})
  return()
endif()
return()
add_custom_command(
  TARGET xTools
  POST_BUILD
  COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:xTools> --no-compiler-runtime
  COMMENT "Deploy Qt for Windows..."
  VERBATIM)
