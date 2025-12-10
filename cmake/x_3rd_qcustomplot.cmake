# https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz
# cmake-format: off
set(QCustomPlot_VERSION "2.1.1" CACHE STRING "Version of QCustomPlot to download")
set(package_url "https://www.qcustomplot.com/release/${QCustomPlot_VERSION}/QCustomPlot-source.tar.gz")
set(package_name "QCustomPlot-source-${QCustomPlot_VERSION}")
# cmake-format: on

if(APPLE)
  return()
endif()

macro(x_remove_all_plot_files)

endmacro()

# --------------------------------------------------------------------------------------------------
# Download the package if it is not existing in 3rd party folder
if(NOT EXISTS "${X_3RD_DIR}/${package_name}.tar.gz")
  message(STATUS "[QCustomPlot] Downloading ${package_name} from ${package_url}")
  file(
    DOWNLOAD "${package_url}" "${X_3RD_DIR}/${package_name}.tar.gz"
    SHOW_PROGRESS
    STATUS download_status)

  list(GET download_status 0 status_code)
  if(NOT status_code EQUAL 0)
    message(WARNING "[QCustomPlot] Download ${package_name} failed. Plot files will be removed.")
    x_remove_all_plot_files()
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Archive extraction
if(NOT EXISTS "${X_3RD_DIR}/${package_name}")
  message(STATUS "[QCustomPlot] Extracting ${package_name}")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf "${X_3RD_DIR}/${package_name}.tar.gz"
    WORKING_DIRECTORY "${X_3RD_DIR}"
    RESULT_VARIABLE extract_result)

  if(NOT extract_result EQUAL 0)
    message(WARNING "[QCustomPlot] Extracting ${package_name} failed. Plot files will be removed.")
    x_remove_all_plot_files()
    return()
  endif()

  # Rename extracted folder to standard name, "qcustomplot-source" to "qcustomplot-source-<version>"
  execute_process(COMMAND ${CMAKE_COMMAND} -E rename "${X_3RD_DIR}/qcustomplot-source"
                          "${X_3RD_DIR}/${package_name}")
endif()

# --------------------------------------------------------------------------------------------------
include_directories(${X_3RD_DIR}/${package_name})
set(out_dir "${X_LIBS_DIR}/${package_name}")
if(EXISTS ${out_dir}/QCustomPlot.lib OR EXISTS ${out_dir}/libQCustomPlot.a)
  link_directories(${out_dir})
  list(APPEND X_LIBS QCustomPlot)
else()
  add_library(QCustomPlot STATIC "${X_3RD_DIR}/${package_name}/qcustomplot.cpp"
                                 "${X_3RD_DIR}/${package_name}/qcustomplot.h")
  target_link_libraries(QCustomPlot PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
  set_target_properties(QCustomPlot PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_dir})
  list(APPEND X_LIBS QCustomPlot)
endif()
