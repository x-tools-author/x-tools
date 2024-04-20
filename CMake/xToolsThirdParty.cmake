set(X_TOOLS_THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)
set(X_TOOLS_STYLES_DIR_NAME "Qt-Advanced-Stylesheets-main")
option(X_TOOLS_ENABLE_MODULE_STYLESHEET "Enable Qt advanced stylesheet" ON)
if(X_TOOLS_ENABLE_MODULE_STYLESHEET)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${X_TOOLS_STYLES_DIR_NAME}.zip
                  WORKING_DIRECTORY ${X_TOOLS_THIRD_PARTY_DIR})
  include_directories(${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_STYLES_DIR_NAME}/src)
  add_compile_definitions(ACSS_STATIC)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_STYLESHEET)
endif()

function(x_tools_copy_style_resources_for_target target)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory_if_different
      "${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_STYLES_DIR_NAME}/styles"
      "$<TARGET_FILE_DIR:${target}>/3rd_styles"
    COMMENT "Copy style resources for ${target}...")
endfunction()

function(x_tools_add_stylesheet_files target)
  set(SOURCE_PATH ${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_STYLES_DIR_NAME}/src)
  list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/acss_globals.h)
  list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/QtAdvancedStylesheet.h)
  list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/QtAdvancedStylesheet.cpp)
  target_sources(${target} PRIVATE ${STYLESHEET_SOURCE})
endfunction()
