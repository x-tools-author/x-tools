# <https://www.qcustomplot.com/release/2.1.1/QCustomPlot.tar.gz>

set(packet_version "2.1.1")
set(packet_url "https://www.qcustomplot.com/release/${packet_version}/QCustomPlot.tar.gz")

# Download the packet if it does not exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/QCustomPlot.tar.gz")
  message(STATUS "Downloading QCustomPlot version ${packet_version}...")
  file(
    DOWNLOAD "${packet_url}" "${CMAKE_SOURCE_DIR}/3rd/QCustomPlot.tar.gz"
    SHOW_PROGRESS
    STATUS status)
  if(NOT status EQUAL 0)
    message(WARNING "Failed to download ${file_name}: ${download_status}")
    return()
  endif()
endif()

# Extract the packet if it does not exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/qcustomplot")
  message(STATUS "Extracting QCustomPlot...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_SOURCE_DIR}/3rd/QCustomPlot.tar.gz"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rd")
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/QCustomPlot")
    message(WARNING "Failed to extract QCustomPlot.")
    return()
  endif()
endif()

if(MSVC)
  set(lib_file "${X_LIBS}/qcustomplot-${packet_version}/QCustomPlot.lib")
else()
  set(lib_file "${X_LIBS}/qcustomplot-${packet_version}/QCustomPlot.a")
endif()

include_directories(${CMAKE_SOURCE_DIR}/3rd/qcustomplot)
get_filename_component(lib_dir ${lib_file} DIRECTORY)
if(EXISTS ${lib_file})
  link_directories(${lib_dir})
else()
  file(GLOB QCUSTOMPLOT_FILES "${CMAKE_SOURCE_DIR}/3rd/qcustomplot/*.*")
  add_library(QCustomPlot STATIC ${QCUSTOMPLOT_FILES})
  target_link_libraries(QCustomPlot Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
                        Qt${QT_VERSION_MAJOR}::Widgets)
  set_target_properties(QCustomPlot PROPERTIES FOLDER "3rd")
  set_target_properties(QCustomPlot PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_dir})
endif()
