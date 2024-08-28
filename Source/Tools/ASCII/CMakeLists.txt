# set(TOOL_OWN_SOURCE "") set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source) x_tools_add_tool_app("ASCII"
# "AsciiAssistant")

x_tools_add_executable(AsciiAssistant ${CMAKE_CURRENT_SOURCE_DIR}/Source/main.cpp
                       ${CMAKE_SOURCE_DIR}/xTools.rc)
x_tools_add_stylesheet_resources(AsciiAssistant)
x_tools_deploy_qt(AsciiAssistant)

target_link_libraries(AsciiAssistant PRIVATE xToolsCore)
