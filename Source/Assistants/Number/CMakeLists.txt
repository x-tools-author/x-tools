set(APP_ASSISTANT_OWN_SOURCE "")
# ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.h ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.cpp
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

x_tools_add_assistant("Number" "NumberAssistant")
x_tools_3rd_setup_glog(NumberAssistant)
