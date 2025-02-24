# https://github.com/QtExcel/QXlsx
set(qxlsx_package_name "QXlsx-1.4.7")

# --------------------------------------------------------------------------------------------------
# Extract QXlsx zip...
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${qxlsx_package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${qxlsx_package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Add QXlsx module...
set(qxlsx_dst_dir ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${qxlsx_package_name})
if(EXISTS ${qxlsx_dst_dir})
  set(CMAKE_PREFIX_PATH ${qxlsx_dst_dir} ${CMAKE_PREFIX_PATH})
  include_directories(${qxlsx_dst_dir}/include/QXlsxQt${QT_VERSION_MAJOR})
  find_package(QXlsxQt${QT_VERSION_MAJOR} REQUIRED)
else()
  add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${qxlsx_package_name}/QXlsx)
endif()

# --------------------------------------------------------------------------------------------------
# Install QXlsx library to libs
function(x_tools_install_qxlsx target)
  add_custom_target(
    ${target}_install_xlsx
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${qxlsx_dst_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${qxlsx_package_name}/QXlsx
    SOURCES ${CMAKE_SOURCE_DIR}/cmake/QXlsx.cmake)
  set_property(TARGET ${target}_install_xlsx PROPERTY FOLDER "install")
endfunction()
