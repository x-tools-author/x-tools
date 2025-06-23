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

# cmake-format: off
set(X_PLOT ON CACHE BOOL "Enable the custom plot library" FORCE)
if(X_PLOT)
  add_compile_definitions(X_PLOT)
  set(tmp "${CMAKE_SOURCE_DIR}/3rd/qcustomplot")
  add_library(QCustomPlot STATIC "${tmp}/qcustomplot.cpp" "${tmp}/qcustomplot.h")
  target_link_libraries(QCustomPlot PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
                        Qt${QT_VERSION_MAJOR}::Widgets)
endif()
# cmake-format: on
