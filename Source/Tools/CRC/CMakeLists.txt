set(ASSISTANT_OWN_SOURCE "")
list(APPEND ASSISTANT_OWN_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsCrcInterface.h)
list(APPEND ASSISTANT_OWN_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsCrcInterface.cpp)

set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)
x_tools_add_assistant_app("CRC" "CrcAssistant")
