# --------------------------------------------------------------------------------------------------
# Qt-Advanced-Stylesheets-main：https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets

find_package(QT NAMES Qt6 QUIET COMPONENTS Core Gui Svg Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Svg Widgets)

set(X_TOOLS_THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/ThirdParty)
set(X_TOOLS_STYLES_DIR_NAME "Qt-Advanced-Stylesheets-main")

execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${X_TOOLS_STYLES_DIR_NAME}.zip
                WORKING_DIRECTORY ${X_TOOLS_THIRD_PARTY_DIR})
include_directories(${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_STYLES_DIR_NAME}/src)
add_compile_definitions(ACSS_STATIC)

set(SOURCE_PATH ${X_TOOLS_THIRD_PARTY_DIR}/${X_TOOLS_STYLES_DIR_NAME}/src)
file(GLOB_RECURSE STYLESHEET_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)
list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/acss_globals.h)
list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/QtAdvancedStylesheet.h)
list(APPEND STYLESHEET_SOURCE ${SOURCE_PATH}/QtAdvancedStylesheet.cpp)

add_library(xQSS STATIC ${STYLESHEET_SOURCE} xQSS.qrc)
target_link_libraries(xQSS PRIVATE Qt6::Core Qt6::Gui Qt6::Svg Qt6::Widgets)
x_tools_generate_translations(xQSS)

# --------------------------------------------------------------------------------------------------
# Copy style resources for ${target}...
function(x_tools_add_style_sheet_resources target)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/${X_TOOLS_STYLES_DIR_NAME}/styles"
      "$<TARGET_FILE_DIR:${target}>/3rd_styles" "||" ${CMAKE_COMMAND} -E true
    COMMENT "Copy style resources for ${target}...")
endfunction()
