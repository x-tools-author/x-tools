file(GLOB_RECURSE SAK_CANBUSSTUDIO_SOURCES "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.h"
     "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.cc" "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.ui")

set(SAK_COMMON_SOURCES
    ${STUDIO_SOURCES}
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.cpp
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.cpp
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.h
    ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.cpp
    ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsUi.h
    ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsMainWindow.h
    ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsMainWindow.cpp
    ${CMAKE_SOURCE_DIR}/xTools.qrc)

set(SAK_CANBUSSTUDIO_SOURCES ${SAK_CANBUSSTUDIO_SOURCES} ${SAK_COMMON_SOURCES})

sak_add_executable("CANBusStudio" ${SAK_CANBUSSTUDIO_SOURCES})
sak_set_target_properties(CANBusStudio)
sak_auto_execute_windeployqt(CANBusStudio)
x_tools_3rd_setup_glog(CANBusStudio)

target_link_libraries(CANBusStudio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                           Qt${QT_VERSION_MAJOR}::SerialBus)
