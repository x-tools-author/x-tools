include(${CMAKE_SOURCE_DIR}/.cmake/sak_common.cmake)

function(sak_auto_execute_windeployqt target)
  if(WIN32)
    set(SAK_WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
    set(QT_CORE_FILE Qt${QT_VERSION_MAJOR}Core${SAK_FILE_SUFFIX}.dll)
    set(depends_dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${QT_CORE_FILE})
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${SAK_WINDEPLOYQT_EXECUTABLE} --qmldir "${CMAKE_SOURCE_DIR}/qml"
              $<TARGET_FILE:${target}> DEPENDS ${depends_dll}
      COMMENT "Running windeployqt...")
  endif()
endfunction()

function(sak_auto_execute_deployqt target)
  if(WIN32)
    sak_auto_execute_windeployqt(${target})
  endif()
endfunction()
