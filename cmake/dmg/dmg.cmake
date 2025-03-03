function(x_tools_generate_dmg target version)
  if(NOT APPLE)
    return()
  endif()

  if(NOT MACDEPLOYQT_EXECUTABLE)
    return()
  endif()

  set(old_name ${target}.dmg)
  set(new_name ${target}-${CMAKE_SYSTEM}-v${version}.dmg)

  add_custom_target(
    ${target}_dmg
    COMMAND ${MACDEPLOYQT_EXECUTABLE} "${target}.app" "-dmg"
    COMMAND ${CMAKE_COMMAND} -E rename "${old_name}" "${new_name}"
    COMMENT "Making dmg..."
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()
