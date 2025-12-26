macro(remove_all_mdns_files)
  file(GLOB_RECURSE MDNS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/mdns/*.*")
  foreach(file ${MDNS_SOURCE})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[xTools.mdns]Remove file: ${file}")
  endforeach(file ${MDNS_SOURCE})
endmacro()

if(QT_VERSION VERSION_LESS "5.9.0")
  remove_all_mdns_files()
  add_compile_definitions(X_DISABLE_MDNS)
  return()
endif()

# --------------------------------------------------------------------------------------------------
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
set(qmdns_dst_dir ${X_LIBS_DIR}/${qmdns_package_name})
if(EXISTS ${qmdns_dst_dir}/include)
  set(CMAKE_PREFIX_PATH ${qmdns_dst_dir} ${CMAKE_PREFIX_PATH})
  find_package(qmdnsengine REQUIRED)
else()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qmdns_package_name})
  set_property(TARGET qmdnsengine PROPERTY FOLDER "3rd")

  add_custom_command(
    OUTPUT ${X_LIBS_DIR}/${qmdns_package_name}/install.stamp
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${X_LIBS_DIR}/${qmdns_package_name}
    COMMAND ${CMAKE_COMMAND} -E touch ${X_LIBS_DIR}/${qmdns_package_name}/install.stamp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${qmdns_package_name}
    COMMENT "Installing qmdnsengine to ${X_LIBS_DIR}/${qmdns_package_name}")
  add_custom_target(qmdnsengine_auto_install ALL
                    DEPENDS ${X_LIBS_DIR}/${qmdns_package_name}/install.stamp)
  add_dependencies(qmdnsengine_auto_install qmdnsengine)
  set_property(TARGET qmdnsengine_auto_install PROPERTY FOLDER "3rd")
endif()

list(APPEND X_LIBS qmdnsengine)
