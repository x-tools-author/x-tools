# * https://github.com/itay-grudev/SingleApplication
# * https://codeload.github.com/itay-grudev/SingleApplication/zip/refs/tags/v3.5.2
set(package_version "3.5.2")
set(package_name "SingleApplication-${package_version}")
set(tmp "https://codeload.github.com/itay-grudev/SingleApplication/zip/refs/tags")
set(package_url "${tmp}/v${package_version}")

# Download the package
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip")
  file(
    DOWNLOAD "${package_url}" "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip"
    SHOW_PROGRESS
    STATUS download_status)
  if(NOT download_status EQUAL 0)
    message(WARNING "Failed to download SingleApplication.")
    return()
  endif()
endif()

# Unzip the package
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}")
  file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip" DESTINATION
       "${CMAKE_CURRENT_SOURCE_DIR}/3rd")
endif()

# Unsupported yet...
return()

set(lib_dir "${X_LIBS_DIR}/${package_name}")
if(EXISTS "${lib_dir}/include")
  set(CMAKE_PREFIX_PATH "${lib_dir}" ${CMAKE_PREFIX_PATH})
  find_package(SingleApplication REQUIRED)
else()
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}")
  set_property(TARGET SingleApplication PROPERTY FOLDER "3rd")

  add_custom_command(
    OUTPUT "${X_LIBS_DIR}/${package_name}/install.stamp"
    COMMAND ${CMAKE_COMMAND} --install . --prefix "${X_LIBS_DIR}/${package_name}"
    COMMAND ${CMAKE_COMMAND} -E touch "${X_LIBS_DIR}/${package_name}/install.stamp"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/3rd/${package_name}"
    COMMENT "Installing SingleApplication to ${X_LIBS_DIR}/${package_name}")
  add_custom_target(SingleApplication_auto_install ALL
                    DEPENDS "${X_LIBS_DIR}/${package_name}/install.stamp")
  add_dependencies(SingleApplication_auto_install SingleApplication)
  set_property(TARGET SingleApplication_auto_install PROPERTY FOLDER "3rd")
  message(STATUS "SingleApplication installed to ${X_LIBS_DIR}/${package_name}")
endif()
