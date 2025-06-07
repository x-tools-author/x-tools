include(cmake/zint.cmake)

file(GLOB_RECURSE XCODE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/xCode)
qt_add_executable(xCode MANUAL_FINALIZATION ${XCODE_SOURCES})
target_link_libraries(xCode PRIVATE Qt6::Core Qt6::Gui Qt6::Svg Qt6::Widgets)
target_link_libraries(xCode PRIVATE QZint)
x_deploy_qt(xCode)
