if(WIN32)
  set(WIN_DEPLOY_QT "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/windeployqt.exe")
  message(STATUS ${Qt${QT_VERSION_MAJOR}_WIN_DEPLOY_QT})
  cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH compiler_path)

  if(MSVC)
    add_custom_target(
      sak_deploy_win
      COMMAND ${WIN_DEPLOY_QT}
              "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QtSwissArmyKnife.exe"
      VERBATIM)
    add_custom_target(
      sak_copy140
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        "${compiler_path}/VCRUNTIME140.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        "${compiler_path}/VCRUNTIME140_1.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${compiler_path}/MSVCP140.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/MSVCP140_1.dll"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/MSVCP140_2.dll"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      VERBATIM)
  elseif(MINGW)
    add_custom_target(
      sak_deploy_win
      COMMAND ${WIN_DEPLOY_QT}
              "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QtSwissArmyKnife.exe"
      VERBATIM)
  endif()

  # Auto deploy for Windows platform.
  option(SAK_AUTO_DEPLOY_WIN "" OFF)
  if(SAK_AUTO_DEPLOY_WIN)
    add_custom_target(sak_auto_deploy_win ALL)
    add_dependencies(sak_auto_deploy_win sak_deploy_win)
  endif()
endif(WIN32)
