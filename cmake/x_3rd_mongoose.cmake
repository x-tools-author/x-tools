# * https://github.com/cesanta/mongoose
# * https://codeload.github.com/cesanta/mongoose/zip/refs/tags/7.20 Do not use version 7.20, OOM
#   error occurs: https://github.com/cesanta/mongoose/issues/3343
set(package_version "7.19")
set(package_name "mongoose-${package_version}")
set(package_url "https://codeload.github.com/cesanta/mongoose/zip/refs/tags/${package_version}")

macro(x_remove_all_x_mqtt_files)
  file(GLOB_RECURSE MQTT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/x/mqtt/*.*")
  foreach(file ${MQTT_SOURCES})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[mongoose] Remove file: ${file}")
  endforeach(file ${MQTT_SOURCES})
endmacro()

option(X_ENABLE_X_MQTT "Enable xMQTT module (requires mongoose)" ON)
if(NOT X_ENABLE_X_MQTT)
  add_compile_definitions(X_ENABLE_X_MQTT=0)
  x_remove_all_x_mqtt_files()
  return()
endif()

# --------------------------------------------------------------------------------------------------
# Download mongoose zip...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip)
  file(
    DOWNLOAD ${package_url} ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip
    SHOW_PROGRESS
    STATUS download_status
    LOG log_mongoose_download)

  list(GET download_status 0 download_result_code)
  if(NOT download_result_code EQUAL 0)
    message(WARNING "[mongoose] Failed to download mongoose")
    # Remove zip file if exists
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip)
      file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip)
    endif()
    # Remove directory if exists
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name})
      file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name})
    endif()
    # Remove xMQTT files
    x_remove_all_x_mqtt_files()
    add_compile_definitions(X_ENABLE_X_MQTT=0)
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Extract mongoose zip...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Import mongoose
include_directories(${X_3RD_DIR}/${package_name})
set(out_dir "${X_LIBS_DIR}/${package_name}")
add_compile_definitions(X_ENABLE_X_MQTT=1)
if(EXISTS ${out_dir}/mongoose.lib OR EXISTS ${out_dir}/libmongoose.a)
  link_directories(${out_dir})
  list(APPEND X_LIBS mongoose)
else()
  add_library(mongoose STATIC "${X_3RD_DIR}/${package_name}/mongoose.c"
                              "${X_3RD_DIR}/${package_name}/mongoose.h")
  list(APPEND X_LIBS mongoose)
  set_target_properties(mongoose PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_dir})
endif()
