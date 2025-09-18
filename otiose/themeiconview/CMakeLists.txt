file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*")
x_add_executable(ThemeIconView ${SRC_FILES})
set_property(TARGET ThemeIconView PROPERTY FOLDER "otiose")
target_link_libraries(ThemeIconView PRIVATE Qt::Gui Qt::Widgets Qt::Core Qt::Svg)
