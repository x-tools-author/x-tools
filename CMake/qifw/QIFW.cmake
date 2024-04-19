function(x_tools_generate_installer target)
  if(NOT WIN32)
    return()
  endif()

  set(args "-DargTarget=${target}")
  list(APPEND args "-D$<TARGET_FILE_DIR:${target}>")
  list(APPEND args "-DargTemperateDir=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Installer")

  add_custom_target(
    ${target}_installer
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/QIFWScript.cmake
    SOURCES ${template}
    WORKING_DIRECTORY ${X_TOOLS_BINARY_DIR}/Installer/${target}
    COMMENT "Start making installer packet for ${target}")
endfunction()
