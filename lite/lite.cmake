# --------------------------------------------------------------------------------------------------
# Find Qt5 components
find_package(
  Qt5
  COMPONENTS Svg Widgets Network SerialPort WebSockets
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
message(STATUS "[xTools] CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
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
# Global settings
set(X_BINS_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/${QT_VERSION}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
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
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(X_ASSET_NAME "xTools-${CMAKE_SYSTEM_NAME}-v${X_LATEST_GIT_TAG}-x64")
else()
  set(X_ASSET_NAME "xTools-${CMAKE_SYSTEM_NAME}-v${X_LATEST_GIT_TAG}-x86")
endif()
set(X_ASSET_NAME "${X_ASSET_NAME}-winxp")
string(TOLOWER ${X_ASSET_NAME} X_ASSET_NAME)
message(STATUS "[xTools] Asset name: ${X_ASSET_NAME}")

# --------------------------------------------------------------------------------------------------
# Get all source files
file(GLOB_RECURSE X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lite/src/*.*)
list(APPEND tmp_dirs "common" "device" utilities)
foreach(tmp_dir ${tmp_dirs})
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/src/${tmp_dir})
  file(GLOB_RECURSE src_files ${dir}/*.*)
  list(APPEND X_SOURCES ${src_files})
endforeach()

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
add_executable(xTools ${X_SOURCES} xTools.qrc xTools.rc)
x_generate_zip(xTools ${X_LATEST_GIT_TAG})
set_target_properties(xTools PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(xTools PRIVATE Dwmapi)
target_link_libraries(xTools PRIVATE Qt5::Svg Qt5::Widgets Qt5::Network Qt5::SerialPort
                                     Qt5::WebSockets)

# --------------------------------------------------------------------------------------------------
# Deploy Qt for Windows
set(WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
if(NOT EXISTS ${WINDEPLOYQT_EXECUTABLE})
  return()
endif()
add_custom_command(
  TARGET xTools
  POST_BUILD
  COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:xTools> --no-compiler-runtime
  COMMENT "Deploy Qt for Windows..."
  VERBATIM)
cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
if(MINGW)
  add_custom_command(
    TARGET xTools
    POST_BUILD VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/libgcc_s_dw2-1.dll"
            $<TARGET_FILE_DIR:xTools> "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/libstdc++-6.dll"
            $<TARGET_FILE_DIR:xTools> "||" ${CMAKE_COMMAND} -E true)
endif()
