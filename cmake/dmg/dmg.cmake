function(x_generate_dmg target version)
  # Just for Qt 6.5 or later
  if(NOT QT_VERSION VERSION_LESS "6.5.0")
    add_custom_target(
      ${target}_dmg
      COMMAND ${MACDEPLOYQT_EXECUTABLE} "${target}.app" "-dmg"
      COMMAND ${CMAKE_COMMAND} -E rename "${target}.dmg" "${X_ASSET_NAME}.dmg" "||" true
      COMMENT "Making dmg..."
      WORKING_DIRECTORY $<TARGET_BUNDLE_DIR:${target}>/..)
  else()
    add_custom_target(
      ${target}_dmg
      COMMAND ${CMAKE_COMMAND} -E echo "Qt 6.5 or later is required for dmg generation."
      COMMENT "Making dmg...")
  endif()
endfunction()
