include(${CMAKE_SOURCE_DIR}/.cmake/sak_common.cmake)

function(sak_auto_execute_windeployqt target)
  if(WIN32)
    set(SAK_WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
    set(QT_CORE_FILE Qt${QT_VERSION_MAJOR}Core${SAK_FILE_SUFFIX}.dll)
    set(depends_dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${QT_CORE_FILE})
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND "${SAK_WINDEPLOYQT_EXECUTABLE}" --qmldir "${CMAKE_SOURCE_DIR}/qml"
              $<TARGET_FILE:${target}> DEPENDS ${depends_dll}
      COMMENT "Running windeployqt..." VERBATIM)
  endif()
endfunction()

function(sak_auto_execute_macdeployqt target)
  if(NOT ${target} STREQUAL "QtSwissArmyKnife")
    return()
  endif()

  if(NOT APPLE)
    return()
  endif()
  
  set(SAK_MACDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/macdeployqt")
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${SAK_MACDEPLOYQT_EXECUTABLE} ${SAK_BINARY_DIR}/${target}/${target}.app -qmldir=${CMAKE_SOURCE_DIR}/qml -dmg
    COMMENT "Running macdeployqt..." VERBATIM)

  add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND sh -c "ls *.dmg > dmgs.txt"
      WORKING_DIRECTORY ${SAK_BINARY_DIR}/${target} COMMENT "Scan dmg file" VERBATIM)

  add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND sh -c "rm qtswissarmyknife-macos-11.dmg || true"
      COMMAND sh -c "cat dmgs.txt | xargs -I {} mv {} qtswissarmyknife-macos-11.dmg"
      COMMAND sh -c "rm dmgs.txt || true"
      WORKING_DIRECTORY ${SAK_BINARY_DIR}/${target} COMMENT "Rename old dmg file" VERBATIM)
endfunction()

function(sak_auto_execute_linuxdeployqt target)
  if(NOT ${target} STREQUAL "QtSwissArmyKnife")
    return()
  endif()

  if(NOT ${SAK_ENABLE_LINUXDEPLOYQT})
    return()
  endif()
  
  find_program(SAK_QMAKE NAMES qmake PATHS ${QT_DIR}/../../../bin)
  set(APP_DIR ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/platform/unix/QtSwissArmyKnife ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/bin
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target}> ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/bin
    COMMAND ${SAK_BIN_LINUXDEPLOYQT} "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/share/applications/QtSwissArmyKnife.desktop" -verbose=0 -appimage -qmake=${SAK_QMAKE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMENT "Running creating appimage file..." VERBATIM)

    if(${BUILD_SHARED_LIBS})
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glog::glog> ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/lib/$<TARGET_FILE_NAME:glog::glog>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMENT "Copy glog to QtSwissArmyKnifeAppDir/lib..." VERBATIM)
      add_custom_command(
       TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/lib/$<TARGET_FILE_NAME:glog::glog> 
                ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/lib/libglog.so.1
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMENT "Copy glog and rename" VERBATIM)
    endif()

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND sh -c "ls *.AppImage > AppImages.txt"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMENT "Scan AppImage file" VERBATIM)

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND sh -c "rm qtswissarmyknife-linux-x86_64.AppImage || true"
        COMMAND sh -c "cat AppImages.txt | xargs -I {} mv {} qtswissarmyknife-linux-x86_64.AppImage"
        COMMAND sh -c "rm AppImages.txt || true"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMENT "Rename old AppImage file" VERBATIM)
endfunction()

function(sak_auto_execute_deployqt target)
  if(WIN32)
    sak_auto_execute_windeployqt(${target})
  elseif (UNIX AND NOT APPLE)
    sak_auto_execute_linuxdeployqt(${target})
  elseif(APPLE)
    sak_auto_execute_macdeployqt(${target})
  endif()
endfunction()
