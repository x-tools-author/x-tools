file(GLOB_RECURSE SAK_CANBUSSTUDIO_SOURCES "${CMAKE_SOURCE_DIR}/src/canbusstudio/*.h"
     "${CMAKE_SOURCE_DIR}/src/canbusstudio/*.cc" "${CMAKE_SOURCE_DIR}/src/canbusstudio/*.ui")

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

set(SAK_CANBUSSTUDIO_SOURCES ${SAK_CANBUSSTUDIO_SOURCES} ${SAK_COMMON_SOURCES})

sak_add_executable("CANBusStudio" ${SAK_CANBUSSTUDIO_SOURCES})
sak_set_target_properties(CANBusStudio)
sak_auto_execute_windeployqt(CANBusStudio)
sak_3rd_setup_glog(CANBusStudio)

target_link_libraries(CANBusStudio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                           Qt${QT_VERSION_MAJOR}::SerialBus)
