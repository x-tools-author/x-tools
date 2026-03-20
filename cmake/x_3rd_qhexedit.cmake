# https://codeload.github.com/Simsys/qhexedit2/zip/refs/tags/v0.9.0
set(package_version "0.9.0")
set(file_name "qhexedit2-${package_version}")
set(file_url "https://codeload.github.com/Simsys/qhexedit2/zip/refs/tags/v${package_version}")

if(QT_VERSION VERSION_GREATER_EQUAL "6.8.0")
  option(X_ENABLE_Q_HEX_EDIT "Enable using QHexEdit library" OFF)
else()
  option(X_ENABLE_Q_HEX_EDIT "Enable QHexEdit library" OFF)
endif()

# --------------------------------------------------------------------------------------------------
# Download and unzip the file if it doesn't exist
if((NOT EXISTS "${X_3RD_DIR}/${file_name}.zip") AND X_ENABLE_Q_HEX_EDIT)
  # Download the file
  message(STATUS "Downloading ${file_name} from ${file_url}")
  file(
    DOWNLOAD "${file_url}" "${X_3RD_DIR}/${file_name}.zip"
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 status_code)
  if(NOT status_code EQUAL 0)
    message(STATUS "[QHexEdit] Failed to download ${file_name}. Status: ${download_status}")
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Unzip the file if it hasn't been unzipped yet
if((NOT EXISTS "${X_3RD_DIR}/${file_name}") AND X_ENABLE_Q_HEX_EDIT)
  # Unzip the file
  message(STATUS "Unzipping ${file_name}.zip")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf "${X_3RD_DIR}/${file_name}.zip"
    WORKING_DIRECTORY "${X_3RD_DIR}"
    RESULT_VARIABLE unzip_result)
  if(NOT unzip_result EQUAL 0)
    message(STATUS "[QHexEdit] Failed to unzip ${file_name}. Result: ${unzip_result}")
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Add Hex Editor to source files
if(X_ENABLE_Q_HEX_EDIT)
  set(src_dir "${X_3RD_DIR}/${file_name}/src")
  include_directories(${src_dir})
  file(GLOB_RECURSE src_files "${src_dir}/*.cpp" "${src_dir}/*.h")
  list(APPEND X_SOURCES ${src_files})
  add_definitions(-DX_ENABLE_Q_HEX_EDIT=1)
else()
  add_definitions(-DX_ENABLE_Q_HEX_EDIT=0)
endif()
