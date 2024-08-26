set(TOOL_OWN_SOURCE "")
list(APPEND TOOL_SOURCES ${CMAKE_SOURCE_DIR}/Source/IO/xIO.h)
list(APPEND TOOL_SOURCES ${CMAKE_SOURCE_DIR}/Source/IO/xIO.cpp)

set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network
                       Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::SerialBus)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)
x_tools_add_tool_app("CANBus" "CANBusAssistant")
