# https://github.com/zint/zint
set(file_name "zint-master")
set(file_url "https://codeload.github.com/zint/zint/zip/refs/heads/master")

set(ZINT_SHARED
    OFF
    CACHE BOOL "Shared Zint" FORCE)
set(ZINT_STATIC
    ON
    CACHE BOOL "Static Zint" FORCE)

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
add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${file_name})
