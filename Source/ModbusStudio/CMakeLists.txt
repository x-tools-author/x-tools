file(GLOB_RECURSE SAK_MODBUSSTUDIO_SOURCES "${CMAKE_SOURCE_DIR}/Source/ModbusStudio/*.h"
     "${CMAKE_SOURCE_DIR}/Source/ModbusStudio/*.cc" "${CMAKE_SOURCE_DIR}/Source/ModbusStudio/*.ui")

set(SAK_COMMON_SOURCES
    ${STUDIO_SOURCES}
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.cpp
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.cpp
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.cpp
    ${CMAKE_SOURCE_DIR}/Source/Common/Commonui/xToolsUi.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Commonui/xToolsMainWindow.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Commonui/xToolsMainWindow.cpp
    ${CMAKE_SOURCE_DIR}/xTools.qrc)

set(SAK_MODBUSSTUDIO_SOURCES ${SAK_MODBUSSTUDIO_SOURCES} ${SAK_COMMON_SOURCES})

sak_add_executable("ModbusStudio" ${SAK_MODBUSSTUDIO_SOURCES})
sak_set_target_properties(ModbusStudio)
sak_auto_execute_windeployqt(ModbusStudio)
x_tools_3rd_setup_glog(ModbusStudio)

target_link_libraries(
  ModbusStudio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialBus
                       Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Network)
