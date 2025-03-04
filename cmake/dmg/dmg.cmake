function(x_tools_generate_dmg target version)
  if(NOT APPLE)
    return()
  endif()

  if(NOT MACDEPLOYQT_EXECUTABLE)
    return()
  endif()

  add_custom_target(
    ${target}_dmg
    COMMAND ${MACDEPLOYQT_EXECUTABLE} "${target}.app" "-dmg"
    COMMAND ${CMAKE_COMMAND} -E rename "${target}.dmg" "${X_ASSET_NAME}.dmg"
    COMMENT "Making dmg..."
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()
