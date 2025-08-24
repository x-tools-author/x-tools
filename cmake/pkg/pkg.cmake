function(x_generate_pkg target version developer_id_application developer_id_installer)
  # Just for Qt 6.5 or later
  if(QT_VERSION VERSION_LESS "6.5.0")
    return()
  endif()

  # cmake-format: off
  set(pkg_root "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}_pkg")
  execute_process(WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} 
    COMMAND ${CMAKE_COMMAND} -E make_directory ${pkg_root})
  add_custom_target(${target}_pkg VERBATIM COMMENT "Making pkg..." WORKING_DIRECTORY ${pkg_root}
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${target}.app" "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "../${target}.app" "${target}.app" "||" ${CMAKE_COMMAND} -E true
    COMMAND ${MACDEPLOYQT_EXECUTABLE} ${target}.app
    COMMAND codesign --deep --force --verbose --sign "${developer_id_application}" "${target}.app"
    COMMAND productbuild --component "${target}.app" /Applications/ --sign "${developer_id_installer}" --product "${target}.app/Contents/Info.plist" "${target}.pkg")
  add_dependencies(${target}_pkg ${target})
  # cmake-format: on
endfunction()
