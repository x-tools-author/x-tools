# https://github.com/nitroshare/qmdnsengine
set(qmdns_package_name "qmdnsengine-master")

# --------------------------------------------------------------------------------------------------
# Extract qmdns zip...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qmdns_package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${qmdns_package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Add qmdns module...
set(qmdns_dst_dir ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${qmdns_package_name})
if(EXISTS ${qmdns_dst_dir})
  set(CMAKE_PREFIX_PATH ${qmdns_dst_dir} ${CMAKE_PREFIX_PATH})
  find_package(qmdnsengine REQUIRED)
else()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qmdns_package_name})
endif()

# --------------------------------------------------------------------------------------------------
# Install qmdns library to libs
function(x_tools_install_qmdns target)
  add_custom_target(
    ${target}_install_qmdns
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${qmdns_dst_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${qmdns_package_name}
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/qmdnsengine.cmake)
  set_property(TARGET ${target}_install_qmdns PROPERTY FOLDER "install")
endfunction()
