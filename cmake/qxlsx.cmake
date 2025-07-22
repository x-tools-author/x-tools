# * https://github.com/QtExcel/QXlsx
# * https://codeload.github.com/QtExcel/QXlsx/zip/refs/tags/v1.4.10
set(qxlsx_version "1.4.10")
set(qxlsx_package_name "QXlsx-${qxlsx_version}")

# --------------------------------------------------------------------------------------------------
# Download QXlsx zip...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qxlsx_package_name}.zip)
  file(DOWNLOAD "https://codeload.github.com/QtExcel/QXlsx/zip/refs/tags/v${qxlsx_version}"
       ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qxlsx_package_name}.zip STATUS download_status)
  if(NOT download_status EQUAL 0)
    set(qxlsx_version "1.4.7")
    set(qxlsx_package_name "QXlsx-${qxlsx_version}")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Extract QXlsx zip...
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${qxlsx_package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${qxlsx_package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Add QXlsx module...
string(TOLOWER ${qxlsx_package_name} qxlsx_package_name_lower)
set(qxlsx_dst_dir ${X_LIBS}/${qxlsx_package_name_lower})
if(EXISTS ${qxlsx_dst_dir}/install.stamp)
  set(CMAKE_PREFIX_PATH ${qxlsx_dst_dir} ${CMAKE_PREFIX_PATH})
  include_directories(${qxlsx_dst_dir}/include/QXlsxQt${QT_VERSION_MAJOR})
  find_package(QXlsxQt${QT_VERSION_MAJOR} REQUIRED)
else()
  add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${qxlsx_package_name_lower}/QXlsx)
  set_property(TARGET QXlsx PROPERTY FOLDER "3rd")

  add_custom_command(
    OUTPUT ${X_LIBS}/${qxlsx_package_name_lower}/install.stamp
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${X_LIBS}/${qxlsx_package_name_lower}
    COMMAND ${CMAKE_COMMAND} -E touch ${X_LIBS}/${qxlsx_package_name_lower}/install.stamp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${qxlsx_package_name_lower}/QXlsx
    COMMENT "Installing QXlsx to ${X_LIBS}/${qxlsx_package_name_lower}")
  add_custom_target(QXlsx_auto_install ALL
                    DEPENDS ${X_LIBS}/${qxlsx_package_name_lower}/install.stamp)
  add_dependencies(QXlsx_auto_install QXlsx)
  set_property(TARGET QXlsx_auto_install PROPERTY FOLDER "3rd")
endif()
