find_package(
  Qt6
  COMPONENTS Widgets Quick QuickControls2
  REQUIRED)

file(GLOB_RECURSE X_SOURCES "v8/src/*.*")
set(X_BIN "${CMAKE_BINARY_DIR}/xTools")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/xTools")
qt_add_executable(xTools ${X_SOURCES} xTools.qrc)
x_deploy_qt(xTools)
x_generate_translations(xTools)
target_link_libraries(xTools PRIVATE ${X_LIBS} Qt6::QuickControls2)
if(MSVC)
  target_link_libraries(xTools PRIVATE Dwmapi)
endif()

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
