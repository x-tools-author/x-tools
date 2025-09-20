# https://codeload.github.com/marella/material-design-icons/zip/refs/tags/v0.14.15
set(package_version "0.14.15")
set(package_file "material-design-icons-${package_version}")
set(package_url
    "https://codeload.github.com/marella/material-design-icons/zip/refs/tags/v${package_version}")

# Download......
if(NOT EXISTS "${X_3RD_DIR}/${package_file}.zip")
  file(
    DOWNLOAD "${package_url}" "${X_3RD_DIR}/${package_file}.zip"
    SHOW_PROGRESS
    STATUS download_status
    LOG log_download)
  list(GET download_status 0 download_status_code)
  if(NOT download_status_code EQUAL 0)
    message(FATAL_ERROR "[xTools-icons] Download of ${package_url} failed: ${log_download}")
  endif()
  message(STATUS "[xTools-icons] Downloaded ${package_url}")
else()
  message(STATUS "[xTools-icons] Using cached ${X_3RD_DIR}/${package_file}.zip")
endif()

# Extract...
if(NOT EXISTS "${X_3RD_DIR}/${package_file}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${X_3RD_DIR}/${package_file}.zip"
                  WORKING_DIRECTORY ${X_3RD_DIR})
  message(STATUS "[xTools-icons] Extracted ${X_3RD_DIR}/${package_file}.zip")
else()
  message(STATUS "[xTools-icons] Using cached ${X_3RD_DIR}/${package_file}")
endif()
