function(x_tools_generate_dmg target version)
  if(NOT APPLE)
    return()
  endif()

  if(NOT MACDEPLOYQT_EXECUTABLE)
    return()
  endif()

  add_custom_target(
    ${target}_dmg
    COMMAND ${MACDEPLOYQT_EXECUTABLE} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}.app" "-dmg"
    COMMENT "Running macdeployqt..."
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

endfunction()
