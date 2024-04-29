set(X_TOOLS_THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty)
# --------------------------------------------------------------------------------------------------
# Qt-Advanced-Stylesheets-main：https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets
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
      "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../ThirdParty/${X_TOOLS_STYLES_DIR_NAME}/styles"
      "$<TARGET_FILE_DIR:${target}>/3rd_styles"
    COMMENT "Copy style resources for ${target}...")
endfunction()

function(x_tools_add_stylesheet_files target)
  if(NOT X_TOOLS_ENABLE_MODULE_STYLESHEET)
    return()
  endif()
  set(SOURCE_PATH ${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_STYLES_DIR_NAME}/src)
  list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/acss_globals.h)
  list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/QtAdvancedStylesheet.h)
  list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/QtAdvancedStylesheet.cpp)
  target_sources(${target} PRIVATE ${STYLESHEET_SOURCE})
endfunction()
# --------------------------------------------------------------------------------------------------
# hidapi: https://github.com/libusb/hidapi
set(X_TOOLS_HID_DIR_NAME "hidapi-hidapi-0.14.0")
option(X_TOOLS_ENABLE_MODULE_HID "Enable HID module" OFF)
if(X_TOOLS_ENABLE_MODULE_HID)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${X_TOOLS_HID_DIR_NAME}.zip
                  WORKING_DIRECTORY ${X_TOOLS_THIRD_PARTY_DIR})
  include_directories(${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_HID_DIR_NAME}/hidapi)
  add_compile_definitions(X_TOOLS_ENABLE_MODULE_HID)
endif()

set(HID_ROOT_DIR ${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_HID_DIR_NAME})
function(x_tools_add_hid_files target)
  if(NOT X_TOOLS_ENABLE_MODULE_HID)
    return()
  endif()

  target_sources(${target} PRIVATE ${HID_ROOT_DIR}/hidapi/hidapi.h)
  if(WIN32)
    target_sources(${target} PRIVATE ${HID_ROOT_DIR}/windows/hid.c)
  elseif(UNIX AND NOT APPLE)
    target_sources(${target} PRIVATE ${HID_ROOT_DIR}/linux/hid.c)
  elseif(APPLE)
    target_sources(${target} PRIVATE ${HID_ROOT_DIR}/mac/hid.c)
  endif()
endfunction()
