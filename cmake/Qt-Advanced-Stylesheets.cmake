# https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets
if(ANDROID OR IOS)
  option(X_TOOLS_ENABLE_QSS "Enable Qt advanced style sheet" OFF)

  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/qss)
  file(GLOB_RECURSE QSS_MGR_SOURCE "${TMP_DIR}/*.*")
  foreach(file ${QSS_MGR_SOURCE})
    message(STATUS "[QSS]Remove file: ${file}")
    list(REMOVE_ITEM X_TOOLS_SOURCE ${file})
  endforeach()
else()
  option(X_TOOLS_ENABLE_QSS "Enable Qt advanced style sheet" ON)
endif()

# --------------------------------------------------------------------------------------------------
# Copy style resources for ${target}...
function(x_tools_deploy_qss target)
  if(NOT X_TOOLS_ENABLE_QSS)
    return()
  endif()

  set(src_dir "${CMAKE_SOURCE_DIR}/3rd/${package_name}/styles")
  set(dst_dir "$<TARGET_FILE_DIR:${target}>/3rd_styles")
  add_custom_target(
    ${target}_qss
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${src_dir} ${dst_dir}
    COMMENT "Copy(manual) style resources for ${target}...")

  # If the directory exists, ignored copying operation...
  if(EXISTS ${dst_dir})
    return()
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${src_dir} ${dst_dir}
    COMMENT "Copy(auto) style resources for ${target}...")
endfunction()

# --------------------------------------------------------------------------------------------------
# If X_TOOLS_ENABLE_QSS is OFF, return...
if(X_TOOLS_ENABLE_QSS)
  add_compile_definitions(X_TOOLS_ENABLE_QSS)
else()
  return()
endif()

# --------------------------------------------------------------------------------------------------
# Extract Qt-Advanced-Stylesheets-main.zip...
set(package_name "Qt-Advanced-Stylesheets-main")
if(NOT ${CMAKE_SOURCE_DIR}/3rd/${package_name}/acss.pri)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# QssMgr library...
add_compile_definitions(ACSS_STATIC)
include_directories(${CMAKE_SOURCE_DIR}/3rd/${package_name}/src)
list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/acss_globals.h)
list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/QtAdvancedStylesheet.h)
list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/QtAdvancedStylesheet.cpp)
list(APPEND QSS_MGR_SOURCE ${CMAKE_SOURCE_DIR}/3rd/${package_name}/src/QtAdvancedStylesheet.cpp)
add_library(QssMgr STATIC ${QSS_MGR_SOURCE})
set_property(TARGET QssMgr PROPERTY FOLDER "3rd")
target_link_libraries(QssMgr PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
                                     Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Widgets)
