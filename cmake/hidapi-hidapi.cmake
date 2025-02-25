# https://github.com/libusb/hidapi.git
set(hidapi_package_name "hidapi-hidapi-0.14.0")

# --------------------------------------------------------------------------------------------------
# Extract hidapi zip...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${hidapi_package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${hidapi_package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Add hidapi module...
set(hidapi_dst_dir ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${hidapi_package_name})
if(EXISTS ${hidapi_dst_dir})
  set(CMAKE_PREFIX_PATH ${hidapi_dst_dir} ${CMAKE_PREFIX_PATH})
  find_package(hidapi REQUIRED)
else()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${hidapi_package_name})
  if(WIN32)
    set_property(TARGET hidapi_winapi PROPERTY FOLDER "3rd")
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
