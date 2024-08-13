set(ASSISTANT_OWN_SOURCE "")
set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)
x_tools_add_assistant_app("Ping" "PingAssistant")
