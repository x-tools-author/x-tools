# https://github.com/QtExcel/QXlsx
set(package_name "QXlsx-1.4.7")

# --------------------------------------------------------------------------------------------------
# Extract QXlsx-1.4.7.zip...
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Add QXlsx module...
set(dst_dir ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${package_name})
if(EXISTS ${dst_dir})
  set(CMAKE_PREFIX_PATH ${dst_dir} ${CMAKE_PREFIX_PATH})
  include_directories(${dst_dir}/include/QXlsxQt${QT_VERSION_MAJOR})
  find_package(QXlsxQt${QT_VERSION_MAJOR} REQUIRED)
else()
  add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${package_name}/QXlsx)
endif()

# --------------------------------------------------------------------------------------------------
# Install QXlsx library to libs
function(x_tools_install_qxlsx target)
  add_custom_target(
    ${target}_install_xlsx
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${dst_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${package_name}/QXlsx)
endfunction()
