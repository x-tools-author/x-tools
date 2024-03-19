set(fontawesome_dir "fontawesome-free-6.5.1-desktop")
add_compile_definitions(SAK_FONTAWESOME_DIR="${fontawesome_dir}")

function(sak_import_fontawesome_for_target target)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory fonts
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory_if_different
      "${CMAKE_SOURCE_DIR}/resources/font/${fontawesome_dir}"
      "${SAK_BINARY_DIR}/${target}/fonts/${fontawesome_dir}"
    WORKING_DIRECTORY "${SAK_BINARY_DIR}/${target}"
    VERBATIM)
endfunction(sak_import_fontawesome_for_target)
