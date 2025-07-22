set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${X_BINS_DIR}/xDemo)
qt_add_executable(xDemo ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
x_deploy_qt(xDemo)
target_link_libraries(xDemo PRIVATE Qt6::Core Qt6::Gui Qt6::Svg Qt6::Widgets)
