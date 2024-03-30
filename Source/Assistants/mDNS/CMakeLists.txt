set(ASSISTANT_OWN_SOURCE "")
list(APPEND ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsDataStructure.h)
list(APPEND ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsDataStructure.cpp)
set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets qmdnsengine)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

x_tools_add_assistant("mDNS" "MDNSAssistant")
