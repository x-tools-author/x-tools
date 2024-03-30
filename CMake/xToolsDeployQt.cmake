set(X_TOOLS_QML_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Qml)

function(x_tools_deploy_qt_for_windows target)
  if(EXISTS "${X_TOOLS_QML_PATH}")
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --qmldir ${X_TOOLS_QML_PATH}
              --no-compiler-runtime
      COMMENT "Deploy Qt (with qml) for Windows..."
      VERBATIM)
  else()
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime
      COMMENT "Deploy Qt for Windows..."
      VERBATIM)
  endif()

  if((${MSVC_VERSION} GREATER_EQUAL 1929) AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/VCRUNTIME140.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/VCRUNTIME140_1.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140_1.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140_2.dll"
              $<TARGET_FILE_DIR:${target}>
      VERBATIM)
  endif()
endfunction()

function(x_tools_deploy_qt_for_mac target)
  if(NOT ${target} STREQUAL "xTools")
    return()
  endif()

  if(NOT APPLE)
    return()
  endif()

  set(SAK_MACDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/macdeployqt")
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/qml")
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${SAK_MACDEPLOYQT_EXECUTABLE} "${X_TOOLS_BINARY_DIR}/${target}/${target}.app"
              "-qmldir=${CMAKE_CURRENT_SOURCE_DIR}/qml -dmg"
      COMMENT "Running macdeployqt..."
      VERBATIM)
  else()
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${SAK_MACDEPLOYQT_EXECUTABLE} "${X_TOOLS_BINARY_DIR}/${target}/${target}.app" "-dmg"
      COMMENT "Running macdeployqt..."
      VERBATIM)
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND sh -c "ls *.dmg > dmgs.txt"
    WORKING_DIRECTORY "${X_TOOLS_BINARY_DIR}/${target}"
    COMMENT "Scan dmg file"
    VERBATIM)

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND sh -c "rm qtswissarmyknife-macos-11.dmg || true"
    COMMAND sh -c "cat dmgs.txt | xargs -I {} mv {} qtswissarmyknife-macos-11.dmg"
    COMMAND sh -c "rm dmgs.txt || true"
    WORKING_DIRECTORY "${X_TOOLS_BINARY_DIR}/${target}"
    COMMENT "Rename old dmg file"
    VERBATIM)
endfunction()

function(x_tools_deploy_qt_for_linux target)
  if(NOT ${target} STREQUAL "xTools")
    return()
  endif()

  if(NOT ${SAK_ENABLE_LINUXDEPLOYQT})
    return()
  endif()

  find_program(
    SAK_QMAKE
    NAMES qmake
    PATHS ${QT_DIR}/../../../bin)
  set(APP_DIR ${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/platform/unix/xTools"
            "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target}>
            "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/bin"
    COMMAND
      ${SAK_BIN_LINUXDEPLOYQT}
      "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/share/applications/xTools.desktop" "-verbose=0"
      "-appimage" "-qmake=${SAK_QMAKE}"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running creating appimage file..."
    VERBATIM)

  if(${BUILD_SHARED_LIBS})
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glog::glog>
              "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/lib/$<TARGET_FILE_NAME:glog::glog>"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Copy glog to QtSwissArmyKnifeAppDir/lib..."
      VERBATIM)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/lib/$<TARGET_FILE_NAME:glog::glog>"
        "${CMAKE_BINARY_DIR}/QtSwissArmyKnifeAppDir/lib/libglog.so.1"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Copy glog and rename"
      VERBATIM)
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND sh -c "ls *.AppImage > AppImages.txt"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Scan AppImage file"
    VERBATIM)

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND sh -c "rm qtswissarmyknife-linux-x86_64.AppImage || true"
    COMMAND sh -c "cat AppImages.txt | xargs -I {} mv {} qtswissarmyknife-linux-x86_64.AppImage"
    COMMAND sh -c "rm AppImages.txt || true"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Rename old AppImage file"
    VERBATIM)
endfunction()

function(x_tools_deploy_qt target)
  if(NOT QT_VERSION_MAJOR EQUAL 6)
    return()
  endif()

  if(NOT ${WINDEPLOYQT_EXECUTABLE})
    return()
  endif()

  if(WIN32)
    x_tools_deploy_qt_for_windows(${target})
  elseif(UNIX AND NOT APPLE)
    #x_tools_deploy_qt_for_linux(${target})
  elseif(APPLE)
    #x_tools_deploy_qt_for_mac(${target})
  endif()
endfunction()
