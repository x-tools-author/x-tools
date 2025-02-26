cmake_minimum_required(VERSION 3.22)
project(
  xTools
  VERSION 8.0
  LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------------------------------------------------------
# Some options for 3rd party libraries
set(WITH_GFLAGS OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL "Reset option" FORCE)
set(WITH_TOOLS
    OFF
    CACHE BOOL "Reset option" FORCE)

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
# CMake module
include(${CMAKE_SOURCE_DIR}/cmake/git.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/glog.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/QXlsx.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/xtools.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/deployqt.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/qifw/qifw.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/qmdnsengine.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/libqrencode.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/hidapi-hidapi.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Qt-Advanced-Stylesheets.cmake)

# --------------------------------------------------------------------------------------------------
# Git module
set(GIT_TAG "x.x.x")
x_tools_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")

# --------------------------------------------------------------------------------------------------
# Get all source files of the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE X_TOOLS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*")

# --------------------------------------------------------------------------------------------------
# QSS module
if(NOT X_TOOLS_ENABLE_QSS)
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/qss)
  file(GLOB_RECURSE QSS_MGR_SOURCE "${TMP_DIR}/*.*")
  foreach(file ${QSS_MGR_SOURCE})
    list(REMOVE_ITEM X_TOOLS_SOURCE ${file})
  endforeach(file ${QSS_MGR_SOURCE})
endif()

# HID
option(X_TOOLS_ENABLE_HID "Enable HID module" OFF)
if(X_TOOLS_ENABLE_HID)
  add_compile_definitions(X_TOOLS_ENABLE_HID)
  set(HIDAPI_WITH_LIBUSB FALSE)
  x_tools_add_third_party("hidapi-hidapi-0.14.0")
endif()
if(NOT X_TOOLS_ENABLE_HID)
  message(STATUS "HID module is disable, HID files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hid.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hid.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hidui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hidui.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hidui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransfermodel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransfermodel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransferui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransferui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidstyledItemdelegate.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidstyledItemdelegate.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransferui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/hidtransferui.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialPort module
option(X_TOOLS_ENABLE_SERIAL_PORT "Enable SerialPort module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialPort)
if(NOT Qt${QT_VERSION_MAJOR}SerialPort_FOUND)
  set(X_TOOLS_ENABLE_SERIAL_PORT OFF)
endif()
if(X_TOOLS_ENABLE_SERIAL_PORT)
  add_compile_definitions(X_TOOLS_ENABLE_SERIAL_PORT)
else()
  message(STATUS "SerialPort module is disable, serial port files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/utilities/serialportscanner.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/utilities/serialportscanner.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/serialport.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/serialport.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/serialportui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/serialportui.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/serialportui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransfermodel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransfermodel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransferui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransferui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialportttyledItemdelegate.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialportttyledItemdelegate.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransferui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/serialporttransferui.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialPort module
option(X_TOOLS_ENABLE_WEB_SOCKET "Enable WebSockets module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS WebSockets)
if(NOT Qt${QT_VERSION_MAJOR}WebSockets_FOUND)
  set(X_TOOLS_ENABLE_WEB_SOCKET OFF)
endif()
if(X_TOOLS_ENABLE_WEB_SOCKET)
  add_compile_definitions(X_TOOLS_ENABLE_WEB_SOCKET)
else()
  message(STATUS "SerialPort module is disable, serial port files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketclient.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketclient.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketclientui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketclientui.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketclientui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketclienttransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketclienttransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketclienttransfermodel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketclienttransfermodel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/websocketclienttransferui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/websocketclienttransferui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/websocketclienttransferui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/websocketclienttransferui.cpp)

  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketserver.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketserver.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketserverui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketserverui.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/websocketserverui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketservertransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketservertransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketservertransfermodel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/transfer/websocketservertransfermodel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/websocketservertransferui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/devicepage/transfer/websocketservertransferui.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# BLE
option(X_TOOLS_ENABLE_BLUETOOTH "Enable Bluetooth module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Bluetooth)
if((NOT Qt${QT_VERSION_MAJOR}Bluetooth_FOUND) OR (QT_VERSION VERSION_LESS 6.5.0))
  set(X_TOOLS_ENABLE_BLUETOOTH OFF)
endif()
if(X_TOOLS_ENABLE_BLUETOOTH)
  add_compile_definitions(X_TOOLS_ENABLE_BLUETOOTH)
else()
  message(STATUS "Bluetooth module is disable, bluetooth files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/utilities/blescanner.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/utilities/blescanner.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/blecentral.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/blecentral.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/blecentralui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/blecentralui.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/blecentralui.cpp)
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
    list(REMOVE_ITEM X_TOOLS_SOURCE ${file})
  endforeach(file ${MODBUS_SOURCE})

  # Remove canbus files
  file(GLOB_RECURSE CANBUS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tools/canbus/*.*")
  foreach(file ${CANBUS_SOURCE})
    list(REMOVE_ITEM X_TOOLS_SOURCE ${file})
  endforeach(file ${CANBUS_SOURCE})
endif()

# --------------------------------------------------------------------------------------------------
# Qt Charts module
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Charts)
if(Qt${QT_VERSION_MAJOR}Charts_FOUND)
  add_compile_definitions(X_TOOLS_ENABLE_CHARTS)
else()
  message(STATUS "Charts module is disable, chart files will be removed.")
  file(GLOB_RECURSE CHARTS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/devicepage/charts/*.*")
  foreach(file ${CHARTS_SOURCE})
    message(STATUS "[Charts]Remove file: ${file}")
    list(REMOVE_ITEM X_TOOLS_SOURCE ${file})
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/xTools)
if(CMAKE_HOST_APPLE)
  set(MACOSX_BUNDLE_ICON_FILE xTools.icns)
  set(xTools_ICON ${CMAKE_SOURCE_DIR}/xTools.icns)
  set_source_files_properties(${xTools_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

qt_add_executable(xTools ${xTools_ICON} ${X_TOOLS_SOURCE} xTools.rc xTools.qrc)
x_tools_generate_translations(xTools)
x_tools_deploy_qt(xTools)
x_tools_tar_target(xTools)
x_tools_deploy_qss(xTools)
x_tools_install_glog(xTools)
x_tools_install_qxlsx(xTools)
x_tools_install_qmdns(xTools)
x_tools_install_qrencode(xTools)
x_tools_install_hidapi(xTools)

set(macos_version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
set(android_dir ${CMAKE_CURRENT_SOURCE_DIR}/android)
set_target_properties(xTools PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER xtools.xtools)
set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING ${macos_version})
set_target_properties(xTools PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${android_dir})

# --------------------------------------------------------------------------------------------------
# Link libraries
target_link_libraries(
  xTools
  PRIVATE Qt${QT_VERSION_MAJOR}::CorePrivate
          Qt${QT_VERSION_MAJOR}::Core
          Qt${QT_VERSION_MAJOR}::Gui
          Qt${QT_VERSION_MAJOR}::Network
          Qt${QT_VERSION_MAJOR}::Svg
          Qt${QT_VERSION_MAJOR}::Widgets
          ${x_qrencode}
          glog::glog
          qmdnsengine)

if(X_TOOLS_ENABLE_SERIAL_PORT)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::SerialPort)
endif()

if(X_TOOLS_ENABLE_WEB_SOCKET)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::WebSockets)
endif()

if(X_TOOLS_ENABLE_SERIALBUS)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::SerialBus)
endif()

if(X_TOOLS_ENABLE_BLUETOOTH)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()

if(X_TOOLS_ENABLE_CHARTS)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::Charts QXlsx::QXlsx)
endif()

if(X_TOOLS_ENABLE_QSS)
  target_link_libraries(xTools PRIVATE QssMgr)
endif()

# --------------------------------------------------------------------------------------------------
# if X_TOOLS_LATEST_GIT_TAG contain "v", remove it...
if(X_TOOLS_LATEST_GIT_TAG MATCHES "^v[0-9].*")
  string(SUBSTRING ${X_TOOLS_LATEST_GIT_TAG} 1 -1 X_TOOLS_LATEST_GIT_TAG)
endif()
if(WIN32)
  x_tools_generate_installer(xTools ${X_TOOLS_LATEST_GIT_TAG}
                             ${CMAKE_CURRENT_SOURCE_DIR}/xTools.ico)
endif()
