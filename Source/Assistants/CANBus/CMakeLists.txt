file(GLOB_RECURSE SOURCES_H "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.h")
file(GLOB_RECURSE SOURCES_UI "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.ui")
file(GLOB_RECURSE SOURCES_CPP "${CMAKE_SOURCE_DIR}/Source/CANBusStudio/*.cpp")

list(APPEND ALL_SOURCE ${SOURCES_H})
list(APPEND ALL_SOURCE ${SOURCES_UI})
list(APPEND ALL_SOURCE ${SOURCES_CPP})
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.h)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.cpp)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.h)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.cpp)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.h)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.cpp)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsUi.h)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsMainWindow.h)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsMainWindow.cpp)
list(APPEND ALL_SOURCE ${CMAKE_SOURCE_DIR}/xTools.qrc)

x_tools_add_executable("CANBusStudio" ${ALL_SOURCE})
x_tools_set_target_properties(CANBusStudio)
x_tools_deploy_qt(CANBusStudio)

target_link_libraries(CANBusStudio PRIVATE glog::glog)
target_link_libraries(CANBusStudio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CANBusStudio PRIVATE Qt${QT_VERSION_MAJOR}::SerialBus)


