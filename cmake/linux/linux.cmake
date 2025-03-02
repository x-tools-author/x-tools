function(x_tools_make_package target packetName friendlyName version is_deb)
  file(GLOB_RECURSE RES_FILES ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/*)

  # target to lower case
  string(TOLOWER ${target} lower_target)
  # Remove all spaces
  string(REGEX REPLACE " " "" lower_target ${lower_target})

  set(package_type "appimage")
  if(${is_deb})
    set(package_type "deb")
  endif()

  set(args "-DargTarget=${target}")
  list(APPEND args "-DargPacketName=${packetName}")
  list(APPEND args "-DargFriendlyName=${friendlyName}")
  list(APPEND args "-DargVersion=${version}")
  list(APPEND args "-DargWorkingDir=${CMAKE_BINARY_DIR}/${package_type}/${target}")
  list(APPEND args "-DargLowerTargetName=${lower_target}")
  list(APPEND args "-DargTool=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/tools/linuxdeployqt")
  list(APPEND args "-DargSrcDir=${CMAKE_SOURCE_DIR}")
  list(APPEND args "-DargPackageType=${package_type}")
  list(APPEND args "-DargQmakePath=${QT_DIR}/../../../bin/qmake")

  # cmake-format: off
  add_custom_target(${target}-${package_type}
    COMMAND ${CMAKE_COMMAND} -E remove_directory {CMAKE_BINARY_DIR}/${package_type}/${target} "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${package_type}/${target} "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/app ${CMAKE_BINARY_DIR}/${package_type}/${target} "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target}> ${CMAKE_BINARY_DIR}/${package_type}/${target}/usr/bin/${packetName}
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/linuxscript.cmake
    SOURCES ${RES_FILES})
  # cmake-format: on
endfunction()
