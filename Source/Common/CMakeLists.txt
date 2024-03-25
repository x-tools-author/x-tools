file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_UI "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(ALL_SOURCE "")
list(APPEND ALL_SOURCE ${X_TOOLS_COMMOM_SOURCE_H})
list(APPEND ALL_SOURCE ${X_TOOLS_COMMOM_SOURCE_UI})
list(APPEND ALL_SOURCE ${X_TOOLS_COMMOM_SOURCE_CPP})

# -------------------------------------------------------------------------------------------------
# BLE module
if(NOT X_TOOLS_BLUETOOTH_MODULE_IS_VALID)
  set(COMMON_PATH ${CMAKE_SOURCE_DIR}/Source/Common)
  list(REMOVE_ITEM ALL_SOURCE ${COMMON_PATH}/Common/xToolsBleScanner.h)
  list(REMOVE_ITEM ALL_SOURCE ${COMMON_PATH}/Common/xToolsBleScanner.cpp)
  list(REMOVE_ITEM ALL_SOURCE ${COMMON_PATH}/CommonUI/xToolsBleDeviceInfoComboBox.h)
  list(REMOVE_ITEM ALL_SOURCE ${COMMON_PATH}/CommonUI/xToolsBleDeviceInfoComboBox.cpp)
  message(STATUS "The common BLE module source has been removed!")
endif()

add_library(xToolsCommon STATIC ${ALL_SOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/xToolsCommon.qrc)
target_link_libraries(xToolsCommon PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(xToolsCommon PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(xToolsCommon PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(xToolsCommon PRIVATE Qt${QT_VERSION_MAJOR}::Network)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  target_link_libraries(xToolsCommon PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
  target_link_libraries(xToolsCommon PRIVATE Qt${QT_VERSION_MAJOR}::SerialBus)
endif()
if(X_TOOLS_BLUETOOTH_MODULE_IS_VALID)
  target_link_libraries(xToolsCommon PRIVATE Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()

x_tools_generate_translations(xToolsCommon)
