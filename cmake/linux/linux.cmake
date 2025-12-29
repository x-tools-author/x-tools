function(x_build_deb target packetName friendlyName version icon)
  file(GLOB_RECURSE RES_FILES ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/*)

  # target to lower case
  string(TOLOWER ${target} lower_target)
  # Remove all spaces
  string(REGEX REPLACE " " "" lower_target ${lower_target})

  set(args "-DargTarget=${target}")
  list(APPEND args "-DargPacketName=${packetName}")
  list(APPEND args "-DargFriendlyName=${friendlyName}")
  list(APPEND args "-DargVersion=${version}")
  list(APPEND args "-DargWorkingDir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../${target}-linux")
  list(APPEND args "-DargIcon=${icon}")
  list(APPEND args "-DargLowerTargetName=${lower_target}")
  if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
    # .github/actions/ci-ubuntu-arm/action.yml will download the tool automatically
    list(APPEND args "-DargTool=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/tools/linuxdeployqt-aarch64")
  else()
    list(APPEND args "-DargTool=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/tools/linuxdeployqt")
  endif()
  list(APPEND args "-DargSrcDir=${CMAKE_SOURCE_DIR}")
  list(APPEND args "-DargQmakePath=${QT_DIR}/../../../bin/qmake")
  list(APPEND args "-DargTargetFile=$<TARGET_FILE:${target}>")
  list(APPEND args "-DargAssetName=${X_ASSET_NAME}")
  if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
    list(APPEND args "-DargSkipGlibcCheck:STRING=TRUE")
  else()
    list(APPEND args "-DargSkipGlibcCheck:STRING=FALSE")
  endif()

  foreach(arg ${args})
    message(STATUS "[deb]arg: ${arg}")
  endforeach()

  # cmake-format: off
  add_custom_target(${target}_linux
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/linuxscript.cmake
    SOURCES ${RES_FILES})
  # cmake-format: on
endfunction()
