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
  if(is_deb)
    list(APPEND args "-DargWorkingDir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}-deb")
  else()
    list(APPEND args "-DargWorkingDir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}-appimage")
  endif()
  list(APPEND args "-DargLowerTargetName=${lower_target}")
  list(APPEND args "-DargTool=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/tools/linuxdeployqt")
  list(APPEND args "-DargSrcDir=${CMAKE_SOURCE_DIR}")
  list(APPEND args "-DargPackageType=${package_type}")
  list(APPEND args "-DargQmakePath=${QT_DIR}/../../../bin/qmake")
  list(APPEND args "-DargTargetFile=$<TARGET_FILE:${target}>")
  list(APPEND args "-DargAssetName=${X_ASSET_NAME}")

  foreach(arg ${args})
    message(STATUS "[${package_type}]arg: ${arg}")
  endforeach()

  # cmake-format: off
  add_custom_target(${target}_${package_type}
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/linuxscript.cmake
    SOURCES ${RES_FILES})
  # cmake-format: on
endfunction()
