file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_UI "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB_RECURSE X_TOOLS_COMMOM_SOURCE_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# -------------------------------------------------------------------------------------------------
# BLE module
if(NOT X_TOOLS_BLUETOOTH_MODULE_IS_VALID)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsBleScanner.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsBleScanner.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Tools/Tools/xToolsBleCentralTool.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Tools/Tools/xToolsBleCentralTool.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE ${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI/xToolsBleCentralToolUi.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE
       ${CMAKE_SOURCE_DIR}/Source/Tools/ToolsUI/xToolsBleCentralToolUi.cpp)
  list(REMOVE_ITEM X_TOOLS_SOURCE
       ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsBleDeviceInfoComboBox.h)
  list(REMOVE_ITEM X_TOOLS_SOURCE
       ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsBleDeviceInfoComboBox.cpp)
  message(STATUS "The BLE module source has been removed!")
endif()

set(ALL_SOURCE "")
list(APPEND ALL_SOURCE ${X_TOOLS_COMMOM_SOURCE_H})
list(APPEND ALL_SOURCE ${X_TOOLS_COMMOM_SOURCE_UI})
list(APPEND ALL_SOURCE ${X_TOOLS_COMMOM_SOURCE_CPP})
list(APPEND ALL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Common.qrc)
add_library(xToolsCommon STATIC ${ALL_SOURCE})
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
