# https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets
if(ANDROID OR IOS)
  option(X_TOOLS_ENABLE_QSS "Enable Qt advanced style sheet" OFF)
else()
  option(X_TOOLS_ENABLE_QSS "Enable Qt advanced style sheet" ON)
endif()
if(X_TOOLS_ENABLE_QSS)
  add_compile_definitions(X_TOOLS_ENABLE_QSS)
else()
  return()
endif()

set(package_name "Qt-Advanced-Stylesheets-main")
if(NOT ${CMAKE_SOURCE_DIR}/3rd/${package_name}/acss.pri)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
endif()

include_directories(${CMAKE_SOURCE_DIR}/3rd/${package_name}/src)
add_compile_definitions(ACSS_STATIC)

list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/acss_globals.h)
list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/QtAdvancedStylesheet.h)
list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/QtAdvancedStylesheet.cpp)
list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/QtAdvancedStylesheet.cpp)
add_library(QssMgr STATIC ${QSS_MGR_SOURCE})
set_property(TARGET QssMgr PROPERTY FOLDER "3rd")
target_link_libraries(QssMgr PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
                                     Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Widgets)

# --------------------------------------------------------------------------------------------------
# Copy style resources for ${target}...
function(x_tools_deploy_qss target)
  add_custom_target(
    ${target}-qss
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory_if_different
      "${CMAKE_SOURCE_DIR}/3rd/${package_name}/styles" "$<TARGET_FILE_DIR:${target}>/3rd_styles"
      "||" ${CMAKE_COMMAND} -E true
    COMMENT "Copy(auto) style resources for ${target}...")

  if(EXISTS "$<TARGET_FILE_DIR:${target}>/3rd_styles")
    return()
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory_if_different
      "${CMAKE_SOURCE_DIR}/3rd/${package_name}/styles" "$<TARGET_FILE_DIR:${target}>/3rd_styles"
      "||" ${CMAKE_COMMAND} -E true
    COMMENT "Copy(manual) style resources for ${target}...")
endfunction()
