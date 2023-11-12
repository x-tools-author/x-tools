function(say_hello_world)
  message(STATUS "Hello world.")
endfunction()

function(sak_add_deploy)
  if(WIN32)
    set(WIN_DEPLOY_QT
        "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/windeployqt.exe")
    message(STATUS ${Qt${QT_VERSION_MAJOR}_WIN_DEPLOY_QT})
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH compiler_path)

    add_custom_target(
      qtswissarmyknife_deploy_win
      COMMAND ${WIN_DEPLOY_QT}
              "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QtSwissArmyKnife.exe"
      VERBATIM)

    if(MSVC)
      add_custom_target(
        sak_copy140
        COMMAND
          ${CMAKE_COMMAND} -E copy_if_different
          "${compiler_path}/VCRUNTIME140.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND
          ${CMAKE_COMMAND} -E copy_if_different
          "${compiler_path}/VCRUNTIME140_1.dll"
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND
          ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/MSVCP140.dll"
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND
          ${CMAKE_COMMAND} -E copy_if_different
          "${compiler_path}/MSVCP140_1.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND
          ${CMAKE_COMMAND} -E copy_if_different
          "${compiler_path}/MSVCP140_2.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        VERBATIM)
    endif()
  endif(WIN32)

  if(WIN32)
    add_custom_target(
      easydebug_deploy_win
      COMMAND ${WIN_DEPLOY_QT} --qmldir "${CMAKE_SOURCE_DIR}/qml"
              "${CMAKE_BINARY_DIR}/EasyDebug/EasyDebug.exe"
      VERBATIM)
  endif(WIN32)

  if(WIN32)
    # Auto deploy for Windows platform.
    option(SAK_AUTO_DEPLOY_WIN "" ON)
    if(SAK_AUTO_DEPLOY_WIN)

    endif()
  endif()
endfunction()
