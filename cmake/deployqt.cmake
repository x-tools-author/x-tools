# --------------------------------------------------------------------------------------------------
# Deploy Qt for Windows
function(x_tools_deploy_qt_for_windows target)
  if(NOT DEFINED WINDEPLOYQT_EXECUTABLE)
    set(WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime
    COMMENT "Deploy Qt for Windows..."
    VERBATIM)

  if(MSVC AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    # add '-' to ignore error if the file does not exist
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND -${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/VCRUNTIME140.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND -${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/VCRUNTIME140_1.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND -${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND -${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140_1.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND -${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/MSVCP140_2.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND -${CMAKE_COMMAND} -E copy_if_different "${QT_DIR}/../../../bin/libcrypto-3-x64.dll"
              $<TARGET_FILE_DIR:${target}>
      COMMAND -${CMAKE_COMMAND} -E copy_if_different "${QT_DIR}/../../../bin/libssl-3-x64.dll"
              $<TARGET_FILE_DIR:${target}>)
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for macOS
function(x_tools_deploy_qt_for_mac target)
  if(NOT MACDEPLOYQT_EXECUTABLE)
    return()
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${MACDEPLOYQT_EXECUTABLE} "${target}.app" "-dmg"
    COMMAND sh -c "ls *.dmg | xargs -I {} mv {} xtools-macos-13.dmg"
    COMMENT "Running macdeployqt..."
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
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
