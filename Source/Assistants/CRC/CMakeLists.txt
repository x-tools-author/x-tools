set(ASSISTANT_OWN_SOURCE "")
list(APPEND ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsCrcInterface.h)
list(APPEND ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsCrcInterface.cpp)
set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

x_tools_add_assistant("CRC" "CrcAssistant")
