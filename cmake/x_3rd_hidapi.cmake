# https://github.com/libusb/hidapi
set(X_3RD_VERSION_HIDAPI
    "0.15.0"
    CACHE STRING "Version of hidapi")
set(package_version ${X_3RD_VERSION_HIDAPI})
set(package_name "hidapi-hidapi-${package_version}")
set(package_url "https://codeload.github.com/libusb/hidapi/zip/refs/tags/hidapi-${package_version}")

macro(remove_all_files_of_hidapi)
  message(STATUS "[xTools-hidapi] Removing all flow files...")
  file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/hid*.*")
  foreach(file ${files})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[xTools-hidapi] Removed file: ${file}")
  endforeach()
  add_compile_definitions(X_ENABLE_HID=0)
endmacro()

# --------------------------------------------------------------------------------------------------
# Not supported on Android or iOS yet
if(ANDROID OR IOS)
  message(STATUS "[xTools-hidapi] HIDAPI is not supported on Android or iOS.")
  remove_all_files_of_hidapi()
  return()
endif()

# --------------------------------------------------------------------------------------------------
# You can disable HIDAPI support by setting X_ENABLE_HID to OFF.
option(X_ENABLE_HID "Enable HIDAPI support" ON)
if(NOT X_ENABLE_HID)
  remove_all_files_of_hidapi()
  return()
endif()

# --------------------------------------------------------------------------------------------------
# Download package
if(NOT EXISTS ${X_3RD_DIR}/${package_name}.zip)
  message(STATUS "Downloading ${package_name}...")
  file(
    DOWNLOAD ${package_url} ${X_3RD_DIR}/${package_name}.zip
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 download_status_code)
  if(NOT download_status_code EQUAL 0)
    message(WARNING "Download ${package_name} failed: ${download_status}")
    file(REMOVE ${X_3RD_DIR}/${package_name}.zip)
    remove_all_files_of_hidapi()
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
set(HIDAPI_WITH_TESTS
    OFF
    CACHE BOOL "Build HIDAPI (unit-)tests" FORCE)
include_directories(${X_3RD_DIR}/${package_name}/hidapi)
add_compile_definitions(X_ENABLE_HID=1)

# --------------------------------------------------------------------------------------------------
if(WIN32)
  set(src_file ${CMAKE_BINARY_DIR}/3rd/${package_name}/src/windows/hidapi.lib)
  set(dst_file ${X_LIBS_DIR}/${package_name}/lib/hidapi.lib)
  if(EXISTS ${src_file})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src_file} ${dst_file})
  endif()

  if(EXISTS ${dst_file})
    link_directories(${X_LIBS_DIR}/${package_name}/lib)
  else()
    x_auto_import_package(${package_name} hidapi)
    set_property(TARGET hidapi_winapi PROPERTY FOLDER "3rd")
  endif()
  list(APPEND X_LIBS hidapi)
elseif(APPLE)
  x_auto_import_package(${package_name} hidapi)
  set_property(TARGET hidapi_darwin PROPERTY FOLDER "3rd")
  list(APPEND X_LIBS hidapi_darwin)
elseif(LINUX AND NOT APPLE)
  x_auto_import_package(${package_name} hidapi_hidraw hidapi_libusb)
  set_property(TARGET hidapi_hidraw PROPERTY FOLDER "3rd")
  set_property(TARGET hidapi_libusb PROPERTY FOLDER "3rd")
  list(APPEND X_LIBS hidapi_hidraw)
else()
  message(WARNING "HIDAPI is not supported on this platform.")
  remove_all_files_of_hidapi()
  return()
endif()
