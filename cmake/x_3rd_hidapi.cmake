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
endmacro()

if(ANDROID OR IOS)
  message(STATUS "[xTools-hidapi] HIDAPI is not supported on Android or iOS.")
  remove_all_files_of_hidapi()
  return()
endif()

if(LINUX AND NOT APPLE)
  message(STATUS "[xTools-hidapi] HIDAPI is not supported on Linux.")
  remove_all_files_of_hidapi()
  return()
endif()

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

set(HIDAPI_WITH_TESTS
    OFF
    CACHE BOOL "Build HIDAPI (unit-)tests" FORCE)
include_directories(${X_3RD_DIR}/${package_name}/hidapi)
x_auto_import_package(${package_name} hidapi)
if(WIN32)
  set_property(TARGET hidapi_winapi PROPERTY FOLDER "3rd")
elseif(APPLE)
  set_property(TARGET hidapi_darwin PROPERTY FOLDER "3rd")
elseif(LINUX)
  set_property(TARGET hidapi_hidraw PROPERTY FOLDER "3rd")
else()
  set_property(TARGET hidapi_netbsd PROPERTY FOLDER "3rd")
endif()

option(X_ENABLE_HID "Enable HIDAPI support" OFF)
if(X_ENABLE_HID)
  add_compile_definitions(X_ENABLE_HID)
  list(APPEND X_LIBS hidapi)
else()
  remove_all_files_of_hidapi()
endif()
