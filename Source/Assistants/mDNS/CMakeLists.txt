set(ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsDataStruct.h)
set(ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsDataStruct.cpp)
set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets qmdnsengine)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

x_tools_add_assistant("mDNS" "MDNSAssistant")
