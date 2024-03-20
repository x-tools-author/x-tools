set(APP_ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsCrcInterface.h
                             ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsCrcInterface.cpp)
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

x_tools_add_assistant("CRC" "CrcAssistant")
x_tools_3rd_setup_glog(CrcAssistant)
