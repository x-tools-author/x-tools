# * https://github.com/obgm/libcoap

macro(x_remove_all_coap_files)
  add_compile_definitions(X_ENABLE_X_COAP=0)
  file(GLOB_RECURSE coap_files ${CMAKE_SOURCE_DIR}/src/x/coap/*.*)
  foreach(file_path ${coap_files})
    message(STATUS "[xTools.Coap]Removing CoAP file: ${file_path}")
    list(REMOVE_ITEM X_SOURCES ${file_path})
  endforeach()
endmacro()

if(ANDROID OR IOS)
  x_remove_all_coap_files()
  return()
endif()

if(NOT WIN32)
  x_remove_all_coap_files()
  return()
endif()

option(X_ENABLE_X_COAP "Enable CoAP support via libcoap" OFF)
if(NOT X_ENABLE_X_COAP)
  x_remove_all_coap_files()
  return()
endif()

# --------------------------------------------------------------------------------------------------
set(package_version 4.3.5a)
set(package_name libcoap-${package_version})
set(package_url https://codeload.github.com/obgm/libcoap/zip/refs/tags/v${package_version})

# Download src file
if(NOT EXISTS ${X_3RD_DIR}/${package_name}.zip)
  message(STATUS "[xTools.Coap]Downloading libcoap ${package_version}...")
  file(
    DOWNLOAD ${package_url} ${X_3RD_DIR}/${package_name}.zip
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 download_result)
  if(NOT download_result EQUAL 0)
    # Remove incomplete file
    file(REMOVE ${X_3RD_DIR}/${package_name}.zip)
    x_remove_all_coap_files()
    message(WARNING "[xTools.Coap]Failed to download libcoap from ${package_url}")
    return()
  endif()
endif()

# Extract src file
if(NOT EXISTS ${X_3RD_DIR}/${package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${package_name}.zip
                  WORKING_DIRECTORY ${X_3RD_DIR})
endif()

if(NOT EXISTS ${X_3RD_DIR}/${package_name})
  x_remove_all_coap_files()
  message(WARNING "[xTools.Coap] Failed to extract libcoap from ${package_name}.zip")
  return()
endif()

if(NOT EXISTS ${X_3RD_DIR}/${package_name}/include/coap_config.h)
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different ${X_3RD_DIR}/${package_name}/coap_config.h.windows
      ${X_3RD_DIR}/${package_name}/include/coap_config.h)
endif()

add_compile_definitions(X_ENABLE_X_COAP=1)
file(GLOB_RECURSE coap_files ${X_3RD_DIR}/${package_name}/src/*.c)
# Remove coap_io_lwip.c, coap_io_contiki.c, coap_io_riot.c
list(FILTER coap_files EXCLUDE REGEX ".*coap_io_lwip\\.c$")
list(FILTER coap_files EXCLUDE REGEX ".*coap_io_contiki\\.c$")
list(FILTER coap_files EXCLUDE REGEX ".*coap_io_riot\\.c$")

include_directories(${X_3RD_DIR}/${package_name}/include)
add_library(libcoap STATIC ${coap_files})
target_link_libraries(libcoap ws2_32)
