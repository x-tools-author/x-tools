file(GLOB_RECURSE SAK_MODBUSSTUDIO_SOURCES "${CMAKE_SOURCE_DIR}/src/modbusstudio/*.h"
     "${CMAKE_SOURCE_DIR}/src/modbusstudio/*.cc" "${CMAKE_SOURCE_DIR}/src/modbusstudio/*.ui")

set(SAK_COMMON_SOURCES
    ${STUDIO_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/common/common/saksettings.h
    ${CMAKE_SOURCE_DIR}/src/common/common/saksettings.cc
    ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.h
    ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.cc
    ${CMAKE_SOURCE_DIR}/src/common/common/saktranslator.h
    ${CMAKE_SOURCE_DIR}/src/common/common/saktranslator.cc
    ${CMAKE_SOURCE_DIR}/src/common/commonui/sakui.h
    ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.h
    ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.cc
    ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)

set(SAK_MODBUSSTUDIO_SOURCES ${SAK_MODBUSSTUDIO_SOURCES} ${SAK_COMMON_SOURCES})

sak_add_executable("ModbusStudio" ${SAK_MODBUSSTUDIO_SOURCES})
sak_set_target_properties(ModbusStudio)
sak_auto_execute_windeployqt(ModbusStudio)
sak_3rd_setup_glog(ModbusStudio)

target_link_libraries(
  ModbusStudio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialBus
                       Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Network)
