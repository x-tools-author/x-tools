# https://github.com/libusb/hidapi.git
set(hidapi_package_name "hidapi-hidapi-0.14.0")

if(ANDROID)
  option(X_TOOLS_ENABLE_HID "Enable HID module" OFF)
else()
  option(X_TOOLS_ENABLE_HID "Enable HID module" OFF)
endif()

if(X_TOOLS_ENABLE_HID)
  add_compile_definitions(X_TOOLS_ENABLE_HID)
  set(HIDAPI_WITH_LIBUSB FALSE)
  # --------------------------------------------------------------------------------------------------
  # Extract hidapi zip...
  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${hidapi_package_name})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${hidapi_package_name}.zip
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Install hidapi library to libs
function(x_tools_install_hidapi target)
  return()

  add_custom_target(
    hidapi_install
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${hidapi_dst_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${hidapi_package_name}/src
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/hidapi-hidapi.cmake)
  set_property(TARGET hidapi_install PROPERTY FOLDER "3rd")
endfunction()

# --------------------------------------------------------------------------------------------------
# Remove hid files...
if(NOT X_TOOLS_ENABLE_HID)
  message(STATUS "HID module is disable, HID files will be removed.")
  set(TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

  file(GLOB_RECURSE HID_FILES ${TMP_DIR}/device/hid*)
  foreach(HID_FILE ${HID_FILES})
    list(REMOVE_ITEM X_TOOLS_SOURCE ${HID_FILE})
    message(STATUS "[HID]Remove file: ${HID_FILE}")
  endforeach()

  file(GLOB_RECURSE HID_FILES ${TMP_DIR}/page/transfer/hid*)
  foreach(HID_FILE ${HID_FILES})
    list(REMOVE_ITEM X_TOOLS_SOURCE ${HID_FILE})
    message(STATUS "[HID]Remove file: ${HID_FILE}")
  endforeach()
endif()

# --------------------------------------------------------------------------------------------------
# Add hidapi module...
if(NOT X_TOOLS_ENABLE_HID)
  return()
endif()

set(hidapi_dst_dir ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${hidapi_package_name})
if(EXISTS ${hidapi_dst_dir})
  set(CMAKE_PREFIX_PATH ${hidapi_dst_dir} ${CMAKE_PREFIX_PATH})
  find_package(hidapi REQUIRED)
else()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${hidapi_package_name})
  if(WIN32)
    set_property(TARGET hidapi_winapi PROPERTY FOLDER "3rd")
  endif()
  if(LINUX)
    set_property(TARGET hidapi_libusb PROPERTY FOLDER "3rd")
    set_property(TARGET hidapi_hidraw PROPERTY FOLDER "3rd")
  endif()
endif()
