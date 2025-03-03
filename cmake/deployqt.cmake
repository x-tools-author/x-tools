function(x_tools_deploy_qt_for_windows target)
  if(NOT DEFINED WINDEPLOYQT_EXECUTABLE)
    set(WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
  endif()

  set(X_TOOLS_QML_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Qml)

  if(EXISTS ${X_TOOLS_QML_PATH})
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --qmldir ${X_TOOLS_QML_PATH}
              --no-compiler-runtime "||" ${CMAKE_COMMAND} -E true
      COMMENT "Deploy Qt (with qml) for Windows..."
      VERBATIM)
  else()
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime "||"
              ${CMAKE_COMMAND} -E true
      COMMENT "Deploy Qt for Windows..."
      VERBATIM)
  endif()

  if(MSVC AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/VCRUNTIME140.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/VCRUNTIME140_1.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140_1.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140_2.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      VERBATIM)
    if(EXISTS "${QT_DIR}/../../../bin/libcrypto-3-x64.dll")
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_DIR}/../../../bin/libcrypto-3-x64.dll"
                $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_DIR}/../../../bin/libssl-3-x64.dll"
                $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
        VERBATIM)
    endif()
  endif()
endfunction()

function(x_tools_deploy_qt_for_mac target)
  if(NOT MACDEPLOYQT_EXECUTABLE)
    return()
  endif()

  set(X_TOOLS_QML_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Qml)

  if(NOT ${target} STREQUAL "xTools")
    return()
  endif()

  if(NOT APPLE)
    return()
  endif()

  if(EXISTS "${X_TOOLS_QML_PATH}")
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${MACDEPLOYQT_EXECUTABLE} "${CMAKE_BINARY_DIR}/${target}/${target}.app"
              "-qmldir=${X_TOOLS_QML_PATH} -dmg"
      COMMENT "Running macdeployqt..."
      VERBATIM)
  else()
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${MACDEPLOYQT_EXECUTABLE} "${CMAKE_BINARY_DIR}/${target}/${target}.app" "-dmg"
      COMMENT "Running macdeployqt..."
      VERBATIM)
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND sh -c "ls *.dmg > dmgs.txt"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${target}"
    COMMENT "Scan dmg file"
    VERBATIM)

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND sh -c "rm xtools-macos-13.dmg || true"
    COMMAND sh -c "cat dmgs.txt"
    COMMAND sh -c "cat dmgs.txt | xargs -I {} mv {} xtools-macos-13.dmg"
    COMMAND sh -c "rm dmgs.txt || true"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${target}"
    COMMENT "Rename old dmg file"
    VERBATIM)
endfunction()

function(x_tools_deploy_qt_for_linux target)
  # Do nothing...
endfunction()

function(x_tools_deploy_qt target)
  if(WIN32)
    x_tools_deploy_qt_for_windows(${target})
  elseif(UNIX AND NOT APPLE)
    x_tools_deploy_qt_for_linux(${target})
  elseif(APPLE)
    x_tools_deploy_qt_for_mac(${target})
  endif()
endfunction()
