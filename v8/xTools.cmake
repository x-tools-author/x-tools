cmake_minimum_required(VERSION 3.26)
project(
  xTools
  VERSION 8.0
  LANGUAGES C CXX
  DESCRIPTION "xTools - All in one toolbox for developers and engineers")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick QuickControls2)
qt_standard_project_setup(REQUIRES 6.8)

# --------------------------------------------------------------------------------------------------
# Git info
include(${CMAKE_SOURCE_DIR}/cmake/x.cmake)
x_git_get_latest_tag(${CMAKE_CURRENT_SOURCE_DIR} "X")
x_git_get_latest_commit(${CMAKE_CURRENT_SOURCE_DIR} "X")
x_git_get_latest_commit_time(${CMAKE_CURRENT_SOURCE_DIR} "X")

# --------------------------------------------------------------------------------------------------
# App setup
file(GLOB_RECURSE cpp_files "v8/src/*.*")
set(X_BIN "${CMAKE_BINARY_DIR}/xTools")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/xTools")
qt_add_executable(xTools ${cpp_files} xTools.qrc)
target_link_libraries(xTools PRIVATE Qt6::Widgets Qt6::Quick Qt6::QuickControls2)

# --------------------------------------------------------------------------------------------------
# QML setup
file(GLOB_RECURSE qml_tmp_files "v8/qml/*.*")
foreach(qml_file ${qml_tmp_files})
  file(RELATIVE_PATH rel_path ${CMAKE_SOURCE_DIR} ${qml_file})
  list(APPEND qml_files ${rel_path})
endforeach()
qt_add_qml_module(
  xTools
  URI
  xToolsQml
  QML_FILES
  ${qml_files}
  OUTPUT_DIRECTORY
  ${X_BIN}/qml/)

set_target_properties(
  xTools
  PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER com.xtools.xtools
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

include(GNUInstallDirs)
install(
  TARGETS xTools
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
