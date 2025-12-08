set(X_SVG_SOURCE ${X_TOOLS_SOURCE})

file(GLOB X_SVG_SOURCE ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
list(APPEND X_SVG_SOURCE ${CMAKE_CURRENT_LIST_DIR}/xSvg.rc)
list(APPEND X_SVG_SOURCE ${CMAKE_CURRENT_LIST_DIR}/xSvg.qrc)

include(cmake/x_qt_deploy.cmake)
qt_add_executable(xSvg ${X_SVG_SOURCE})
x_deploy_qt(xSvg)

target_link_libraries(xSvg PRIVATE Qt::Core Qt::Gui Qt::Widgets)

# --------------------------------------------------------------------------------------------------
# Generate installer(exe)
set(root_dir "${CMAKE_BINARY_DIR}/installer/xSvg")
set(icon ${CMAKE_CURRENT_LIST_DIR}/xSvg.ico)
# x_tools_generate_installer(xSvg xSvg ${X_SVG_VERSION})

# --------------------------------------------------------------------------------------------------
# Generate Microsoft Store package(MSIX) if(WIN32) x_tools_generate_msix(xSvg "xSvg" "xSvg"
# "${X_SVG_VERSION}" OFF) endif() x_tools_generate_translations(xSvg)
