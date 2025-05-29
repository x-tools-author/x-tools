function(x_generate_dmg target version)
  if(NOT APPLE)
    return()
  endif()

  if(NOT MACDEPLOYQT_EXECUTABLE)
    return()
  endif()

  # Just for Qt 6.5 or later
  if(QT_VERSION VERSION_LESS "6.5.0")
    add_custom_target(
      ${target}_dmg
      COMMAND ${CMAKE_COMMAND} -E echo "Qt 6.5 or later is required for DMG generation."
      COMMENT "Making dmg..."
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  else()
    add_custom_target(
      ${target}_dmg
      COMMAND ${MACDEPLOYQT_EXECUTABLE} "${target}.app" "-dmg"
      COMMAND ${CMAKE_COMMAND} -E rename "${target}.dmg" "${X_ASSET_NAME}.dmg"
      COMMENT "Making dmg..."
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endif()
endfunction()
