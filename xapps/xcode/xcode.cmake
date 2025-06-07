include(cmake/zint.cmake)

file(GLOB_RECURSE XCODE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
qt_add_executable(xcode MANUAL_FINALIZATION ${XCODE_SOURCES})
target_link_libraries(xcode PRIVATE Qt6::Core Qt6::Gui Qt6::Svg Qt6::Widgets)
target_link_libraries(xcode PRIVATE zint-static)
