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
include(${CMAKE_SOURCE_DIR}/cmake/QIFW/QIFW.cmake)
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
option(X_TOOLS_ENABLE_MODULE_HID "Enable HID module" OFF)
if(X_TOOLS_ENABLE_MODULE_HID)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_HID)
  set(HIDAPI_WITH_LIBUSB FALSE)
  x_tools_add_third_party("hidapi-hidapi-0.14.0")
endif()
if(NOT X_TOOLS_ENABLE_MODULE_HID)
  message(STATUS "HID module is disable, HID files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hid.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hid.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hidui.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hidui.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/device/hidui.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/HidTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidStyledItemDelegate.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidStyledItemDelegate.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/HidTransferUi.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialPort module
option(X_TOOLS_ENABLE_MODULE_SERIAL_PORT "Enable SerialPort module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialPort)
if(NOT Qt${QT_VERSION_MAJOR}SerialPort_FOUND)
  set(X_TOOLS_ENABLE_MODULE_SERIAL_PORT OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIAL_PORT)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_SERIAL_PORT)
else()
  message(STATUS "SerialPort module is disable, serial port files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/Unit/SerialPortScanner.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/Unit/SerialPortScanner.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/SerialPort.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/SerialPort.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/SerialPortUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/SerialPortUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/SerialPortUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/SerialPortTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortStyledItemDelegate.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortStyledItemDelegate.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/SerialPortTransferUi.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Qt SerialPort module
option(X_TOOLS_ENABLE_MODULE_WEB_SOCKET "Enable WebSockets module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS WebSockets)
if(NOT Qt${QT_VERSION_MAJOR}WebSockets_FOUND)
  set(X_TOOLS_ENABLE_MODULE_WEB_SOCKET OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_WEB_SOCKET)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_WEB_SOCKET)
else()
  message(STATUS "SerialPort module is disable, serial port files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketClient.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketClient.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketClientUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketClientUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketClientUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketClientTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketClientTransferUi.cpp)

  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketServer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Communication/WebSocketServer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketServerUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketServerUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Communication/WebSocketServerUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransfer.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransfer.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransferModel.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/IO/Transfer/WebSocketServerTransferModel.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/UI/Transfer/WebSocketServerTransferUi.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# BLE
option(X_TOOLS_ENABLE_MODULE_BLUETOOTH "Enable Bluetooth module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Bluetooth)
if((NOT Qt${QT_VERSION_MAJOR}Bluetooth_FOUND) OR (QT_VERSION VERSION_LESS 6.5.0))
  set(X_TOOLS_ENABLE_MODULE_BLUETOOTH OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_BLUETOOTH)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_BLUETOOTH)
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
option(X_TOOLS_ENABLE_MODULE_SERIALBUS "Enable SerialBus module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS SerialBus)
if(NOT Qt${QT_VERSION_MAJOR}SerialBus_FOUND)
  set(X_TOOLS_ENABLE_MODULE_SERIALBUS OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_SERIALBUS)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Modbus/src)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_MODBUS)

  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_MODBUS_DIR}/main.cpp)
endif()
if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/CANBus/src)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_CANBUS)

  list(REMOVE_ITEM X_TOOLS_SOURCE ${X_TOOLS_CANBUS_DIR}/main.cpp)
endif()

# --------------------------------------------------------------------------------------------------
# Qt Charts module
option(X_TOOLS_ENABLE_MODULE_CHARTS "Enable Charts module" ON)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Charts)
if(NOT Qt${QT_VERSION_MAJOR}Charts_FOUND)
  set(X_TOOLS_ENABLE_MODULE_CHARTS OFF)
endif()
if(X_TOOLS_ENABLE_MODULE_CHARTS)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_CHARTS)
  # x_tools_add_xlsx()
else()
  message(STATUS "Charts module is disable, chart files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/IO)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/DataVisualization/2D/Charts.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/IO/DataVisualization/2D/Charts.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUi.ui)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUiSettings.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUiSettings.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${TMP_DIR}/UI/DataVisualization/2D/ChartsUiSettings.ui)
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
# x_tools_deploy_qss(xTools)
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
          glog::glog
          ${x_qrencode}
          qmdnsengine)

if(X_TOOLS_ENABLE_MODULE_SERIAL_PORT)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::SerialPort)
endif()

if(X_TOOLS_ENABLE_MODULE_WEB_SOCKET)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::WebSockets)
endif()

if(X_TOOLS_ENABLE_MODULE_SERIALBUS)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::SerialBus)
endif()

if(X_TOOLS_ENABLE_MODULE_BLUETOOTH)
  target_link_libraries(xTools PUBLIC Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()

if(X_TOOLS_ENABLE_MODULE_CHARTS)
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
