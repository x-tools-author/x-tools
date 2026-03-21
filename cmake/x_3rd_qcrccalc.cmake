# https://codeload.github.com/KoynovStas/QCRC_Calc/zip/refs/tags/v2.3
set(package_version "2.3")
set(file_name "QCRC_Calc-${package_version}")
set(file_url "https://codeload.github.com/KoynovStas/QCRC_Calc/zip/refs/tags/v${package_version}")

if(QT_VERSION VERSION_GREATER_EQUAL "6.8.0")
  option(X_USING_QCRC_CALC "Enable using QCRC_Calc library" ON)
else()
  option(X_USING_QCRC_CALC "Enable using QCRC_Calc library" OFF)
endif()

# --------------------------------------------------------------------------------------------------
# Download and unzip the file if it doesn't exist
if((NOT EXISTS "${X_3RD_DIR}/${file_name}.zip") AND X_USING_QCRC_CALC)
  # Download the file
  message(STATUS "Downloading ${file_name} from ${file_url}")
  file(
    DOWNLOAD "${file_url}" "${X_3RD_DIR}/${file_name}.zip"
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 status_code)
  if(NOT status_code EQUAL 0)
    file(REMOVE "${X_3RD_DIR}/${file_name}.zip")
    message(STATUS "[QCRC_Calc] Failed to download ${file_name}. Status: ${download_status}")
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Unzip the file if it hasn't been unzipped yet
if((NOT EXISTS "${X_3RD_DIR}/${file_name}") AND X_USING_QCRC_CALC)
  # Unzip the file
  message(STATUS "Unzipping ${file_name}.zip")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf "${X_3RD_DIR}/${file_name}.zip"
    WORKING_DIRECTORY "${X_3RD_DIR}"
    RESULT_VARIABLE unzip_result)
  if(NOT unzip_result EQUAL 0)
    message(STATUS "[QCRC_Calc] Failed to unzip ${file_name}. Result: ${unzip_result}")
    file(REMOVE "${X_3RD_DIR}/${file_name}.zip")
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Add the library target
if(X_USING_QCRC_CALC)
  include_directories("${X_3RD_DIR}/${file_name}/src")
  if((EXISTS "${X_LIBS_DIR}/${file_name}/QCRC_Calc.lib")
     OR (EXISTS "${X_LIBS_DIR}/${file_name}/libQCRC_Calc.a"))
    link_directories("${X_LIBS_DIR}/${file_name}")
  else()
    file(GLOB_RECURSE src_files "${X_3RD_DIR}/${file_name}/src/ucrc_t.*"
         "${X_3RD_DIR}/${file_name}/src/qucrc_t.*")
    add_library(QCRC_Calc STATIC ${src_files})
    target_link_libraries(QCRC_Calc Qt${QT_VERSION_MAJOR}::Qml)
    set_target_properties(
      QCRC_Calc
      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${X_LIBS_DIR}/${file_name}"
                 LIBRARY_OUTPUT_DIRECTORY "${X_LIBS_DIR}/${file_name}"
                 RUNTIME_OUTPUT_DIRECTORY "${X_LIBS_DIR}/${file_name}")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Set the option to enable using QCRC_Calc library
if(X_USING_QCRC_CALC)
  list(APPEND X_LIBS QCRC_Calc)
  add_definitions(-DX_USING_QCRC_CALC=1)
else()
  add_definitions(-DX_USING_QCRC_CALC=0)
endif()
