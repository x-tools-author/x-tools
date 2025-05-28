function(x_generate_msix target packet_name display_name packet_version rename_target)
  set(root_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}-msix)
  set(template ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/template.xml)
  set(args -DargTarget=${target})
  list(APPEND args -DargRoot=${root_dir})
  list(APPEND args -DargTemplate=${template})
  list(APPEND args -DargPacketName=${packet_name})
  list(APPEND args -DargPacketVersion=${packet_version})
  list(APPEND args -DargPackageDisplayName=${display_name})
  list(APPEND args -DargRenameTarget:BOOL=${rename_target})
  message(STATUS "root_dir(${target}): ${root_dir}")
  message(STATUS "template(${target}): ${template}")
  set(msix_source ${template})
  list(APPEND msix_source ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/msix.cmake)
  list(APPEND msix_source ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/msixscript.cmake)

  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${target}-msix
                  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../)

  add_custom_target(
    ${target}_msix
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/msixscript.cmake
    SOURCES ${msix_source}
    DEPENDS ${target}
    WORKING_DIRECTORY ${root_dir}
    COMMENT "Start making msix packet for ${target}")
endfunction()
