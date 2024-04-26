cmake_minimum_required(VERSION 3.21)

project(
  xTools
  VERSION 1.0
  LANGUAGES C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/xTools.cmake)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Common)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Tools)

# --------------------------------------------------------------------------------------------------
# Git module
x_tools_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
x_tools_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X_TOOLS")
set(X_TOOLS_VERSION
    ${GIT_TAG}
    CACHE STRING "The version of applicaion")

# --------------------------------------------------------------------------------------------------
# xTools application
option(X_TOOLS_ENABLE_TARGET_XTOOLS "Enable xTools target" ON)
if(X_TOOLS_ENABLE_TARGET_XTOOLS)
  file(GLOB TMP "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.*")
  file(GLOB_RECURSE X_TOOLS_TOOLBOX_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Source/ToolBox/*.*")
  list(APPEND X_TOOLS_SOURCE ${TMP})
  list(APPEND X_TOOLS_SOURCE xTools.qrc)
  list(APPEND X_TOOLS_SOURCE ${X_TOOLS_TOOLBOX_SOURCE})
  if(WIN32)
    list(APPEND X_TOOLS_SOURCE xTools.rc)
  endif()

  x_tools_add_executable(xTools ${X_TOOLS_SOURCE})
  add_dependencies(xTools xToolsTools)
  x_tools_copy_style_resources_for_target(xTools)
  x_tools_generate_translations(xTools)
  x_tools_deploy_qt(xTools)
  x_tools_tar_target(xTools)
  x_tools_generate_installer(xTools ${X_TOOLS_VERSION})

  target_link_libraries(xTools PRIVATE xToolsTools)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Svg)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Core)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::Network)
  target_link_libraries(xTools PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)

  if(X_TOOLS_ENABLE_MODULE_QRCODE)
    target_link_libraries(xTools PRIVATE qrencode)
  endif()
  if(X_TOOLS_ENABLE_MODULE_MDNS)
    target_link_libraries(xTools PRIVATE qmdnsengine)
  endif()
endif()

# -------------------------------------------------------------------------------------------------
# Assistant applications
option(X_TOOLS_ENABLE_TARGET_ASSISTANTS "Enable assistant applications" OFF)
if(X_TOOLS_ENABLE_TARGET_ASSISTANTS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Assistants)
endif()
