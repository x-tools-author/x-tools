# https://github.com/zint/zint
set(file_name "zint-master")
set(file_url "https://codeload.github.com/zint/zint/zip/refs/heads/master")

# cmake-format: off
set(ZINT_SHARED OFF CACHE BOOL "Shared Zint" FORCE)
set(ZINT_STATIC ON CACHE BOOL "Static Zint" FORCE)
set(ZINT_USE_QT ON CACHE BOOL "Use Qt" FORCE)
set(ZINT_QT6 ON CACHE BOOL "Use Qt6" FORCE)
# cmake-format: on

# Download and unzip the Zint repository if it does not exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/${file_name}.zip")
  message(STATUS "Downloading Zint repository from ${file_url}...")
  file(
    DOWNLOAD "${file_url}" "${CMAKE_SOURCE_DIR}/3rd/${file_name}.zip"
    SHOW_PROGRESS
    STATUS download_status)
  if(NOT download_status EQUAL 0)
    message(FATAL_ERROR "Failed to download Zint repository: ${download_status}")
    return()
  endif()
endif()

# Unzip the Zint repository if it does not exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/${file_name}")
  message(STATUS "Unzipping Zint repository...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${file_name}.zip"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rd")
endif()

# Add the Zint subdirectory to the project
include_directories(${CMAKE_SOURCE_DIR}/3rd/${file_name})
add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${file_name})

# Specify the destination directory for Zint
set_property(TARGET QZint PROPERTY FOLDER "3rd")
set_property(TARGET uninstall PROPERTY FOLDER "3rd")
set_property(TARGET zint_bundled_getopt PROPERTY FOLDER "3rd")
set_property(TARGET zint_frontend PROPERTY FOLDER "3rd")
set_property(TARGET zint-qt PROPERTY FOLDER "3rd")
set_property(TARGET zint-static PROPERTY FOLDER "3rd")
