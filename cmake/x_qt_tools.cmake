# https://codeload.github.com/qt/qttools/zip/refs/tags/v6.11.0
set(package_version ${QT_VERSION})
set(package_file_name qttools-${package_version})
set(package_url https://codeload.github.com/qt/qttools/zip/refs/tags/v${package_version})

if(QT_VERSION VERSION_LESS "6.5.0")
  return()
endif()

# --------------------------------------------------------------------------------------------------
# Download source code
if(NOT EXISTS ${X_3RD_DIR}/qttools-${package_version}.zip)
  file(
    DOWNLOAD ${package_url} ${X_3RD_DIR}/qttools-${package_version}.zip
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 download_result)
  if(NOT download_result EQUAL 0)
    file(REMOVE ${X_3RD_DIR}/qttools-${package_version}.zip)
    message(FATAL_ERROR "Failed to download ${package_file_name}.zip: ${download_status}")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Extract source code
if(NOT EXISTS ${X_3RD_DIR}/${package_file_name})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${X_3RD_DIR}/qttools-${package_version}.zip
    WORKING_DIRECTORY ${X_3RD_DIR}
    RESULT_VARIABLE extract_result)
  if(NOT extract_result EQUAL 0)
    file(REMOVE ${X_3RD_DIR}/qttools-${package_version})
    message(FATAL_ERROR "Failed to extract ${package_file_name}.zip")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Add to project
set(lib_dir ${X_LIBS_DIR}/${package_file_name})
include_directories(${X_3RD_DIR}/${package_file_name}/src/shared/qtpropertybrowser)
if(EXISTS ${lib_dir}/QtPropertyBrowser.lib
   OR EXISTS ${lib_dir}/libQtPropertyBrowser.a
   OR EXISTS ${lib_dir}/QtPropertyBrowser.a)
  link_directories(${lib_dir})
  list(APPEND XLIBS QtPropertyBrowser)
else()
  file(GLOB_RECURSE QtPropertyBrowser_SRC
       ${X_3RD_DIR}/${package_file_name}/src/shared/qtpropertybrowser/*.cpp
       ${X_3RD_DIR}/${package_file_name}/src/shared/qtpropertybrowser/*.h)
  add_library(QtPropertyBrowser STATIC ${QtPropertyBrowser_SRC})
  target_link_libraries(QtPropertyBrowser PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
  set_target_properties(
    QtPropertyBrowser
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${X_LIBS_DIR}/${package_file_name}"
               LIBRARY_OUTPUT_DIRECTORY "${X_LIBS_DIR}/${package_file_name}"
               RUNTIME_OUTPUT_DIRECTORY "${X_LIBS_DIR}/${package_file_name}")
  set_target_properties(QtPropertyBrowser PROPERTIES FOLDER "3rd")
  list(APPEND XLIBS QtPropertyBrowser)
endif()
