set(X_SVG_SOURCE ${X_TOOLS_SOURCE})

file(GLOB X_SVG_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.*)
list(APPEND X_SVG_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/xSvg.rc)
list(APPEND X_SVG_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/xSvg.qrc)

x_tools_add_executable(xSvg ${X_SVG_SOURCE})
x_tools_add_stylesheet_resources(xSvg)
x_tools_deploy_qt(xSvg)
x_tools_tar_target(xSvg)
x_tools_copy_microsoft_store_dll(xSvg)
x_tools_get_latest_version_from_tag(xSvg X_SVG_VERSION)

target_link_libraries(xSvg PRIVATE xApp)

# --------------------------------------------------------------------------------------------------
# Generate installer(exe)
set(root_dir "${CMAKE_BINARY_DIR}/installer/xSvg")
set(icon ${CMAKE_CURRENT_SOURCE_DIR}/xSvg.ico)
x_tools_generate_installer(xSvg xSvg ${X_SVG_VERSION})

# --------------------------------------------------------------------------------------------------
# Generate Microsoft Store package(MSIX)
if(WIN32)
  x_tools_generate_msix(xSvg "xSvg" "xSvg" "${X_SVG_VERSION}" OFF)
endif()
x_tools_generate_translations(xSvg)

set_target_properties(xSvg PROPERTIES FOLDER "xSvg")
set_target_properties(xSvg_lupdate PROPERTIES FOLDER "xSvg")
set_target_properties(xSvg_lrelease PROPERTIES FOLDER "xSvg")
set_target_properties(xSvg_lupgrade PROPERTIES FOLDER "xSvg")
set_target_properties(xSvg_installer PROPERTIES FOLDER "xSvg")
set_target_properties(xSvg_msix PROPERTIES FOLDER "xSvg")
