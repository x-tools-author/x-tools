function(x_generate_pkg target version developer_id_application developer_id_installer)
  # Just for Qt 6.5 or later
  if(QT_VERSION VERSION_LESS "6.5.0")
    return()
  endif()

  set(info_plist "${target}.app/Contents/Info.plist")
  set(pkg_root "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}_pkg")

  # cmake-format: off
  add_custom_target(
    ${target}_pkg
    VERBATIM
    COMMENT "Making pkg..."
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${pkg_root}/${target}.app" "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E make_directory "${pkg_root}" "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${target}.app "${pkg_root}/${target}.app" "||" ${CMAKE_COMMAND} -E true
    COMMAND ${MACDEPLOYQT_EXECUTABLE} "${pkg_root}/${target}.app"
    COMMAND codesign --deep --force --verbose --sign "${developer_id_application}" "${pkg_root}/${target}.app"
    COMMAND productbuild --component "${pkg_root}/${target}.app" /Applications/ --sign "${developer_id_installer}" --product "${info_plist}" "${target}.pkg")
  # cmake-format: on
endfunction()
