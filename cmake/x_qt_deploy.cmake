# --------------------------------------------------------------------------------------------------
# Deploy Qt for Windows
function(x_deploy_qt_for_windows target)
  get_target_property(QT_TARGET_TYPE Qt${QT_VERSION_MAJOR}::Core TYPE)
  if(QT_TARGET_TYPE STREQUAL "STATIC_LIBRARY")
    return()
  endif()

  if(NOT DEFINED WINDEPLOYQT_EXECUTABLE)
    set(WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
  endif()

  if(NOT EXISTS ${WINDEPLOYQT_EXECUTABLE})
    return()
  endif()

  message(STATUS "[xTools.Linguist] WINDEPLOYQT_EXECUTABLE:${WINDEPLOYQT_EXECUTABLE}")
  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/qml)
    message(STATUS "[xTools.Linguist] Deploy Qt with QML dir: ${CMAKE_CURRENT_LIST_DIR}/qml")
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime --qmldir
                "${CMAKE_CURRENT_LIST_DIR}/qml"
        COMMENT "Deploy Qt for Windows..."
        VERBATIM)
    else()
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime --qmldir
                "${CMAKE_CURRENT_LIST_DIR}/qml" --release
        COMMENT "Deploy Qt for Windows..."
        VERBATIM)
    endif()
  else()
    message(STATUS "[xTools.Linguist] Deploy Qt without QML dir")
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime
        COMMENT "Deploy Qt for Windows..."
        VERBATIM)
    else()
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime --release
        COMMENT "Deploy Qt for Windows..."
        VERBATIM)
    endif()
  endif()

  if(MSVC AND NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    # add '-' to ignore error if the file does not exist
    add_custom_command(
      TARGET ${target}
      POST_BUILD VERBATIM
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/vcruntime140.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/vcruntime140_1.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/msvcp140.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/msvcp140_1.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/msvcp140_2.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true)
    if(EXISTS "${QT_DIR}/../../../bin/libcrypto-3-x64.dll")
      add_custom_command(
        TARGET ${target}
        POST_BUILD VERBATIM
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_DIR}/../../../bin/libcrypto-3-x64.dll"
                $<TARGET_FILE_DIR:${target}>)
    endif()
    if(EXISTS "${QT_DIR}/../../../bin/libssl-3-x64.dll")
      add_custom_command(
        TARGET ${target}
        POST_BUILD VERBATIM
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_DIR}/../../../bin/libssl-3-x64.dll"
                $<TARGET_FILE_DIR:${target}>)
    endif()
  endif()
  if(MINGW)
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    # add '-' to ignore error if the file does not exist
    add_custom_command(
      TARGET ${target}
      POST_BUILD VERBATIM
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/libstdc++-6.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/libgcc_s_dw2-1.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COMPILER_PATH}/libwinpthread-1.dll"
              $<TARGET_FILE_DIR:${target}> "||" ${CMAKE_COMMAND} -E true)
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for macOS
function(x_deploy_qt_for_mac target)
  if(NOT DEFINED MACDEPLOYQT_EXECUTABLE)
    return()
  endif()

  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Target path of macOS is: $<TARGET_BUNDLE_DIR:${target}>"
    COMMAND ${MACDEPLOYQT_EXECUTABLE} "$<TARGET_BUNDLE_DIR:${target}>"
    COMMENT "Deploying...")
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for linux
function(x_deploy_qt_for_linux target)
  # Do nothing...
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for unix
function(x_deploy_qt_for_unix target)
  # Do nothing...
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt
function(x_deploy_qt target)
  if(WIN32)
    x_deploy_qt_for_windows(${target})
  elseif(APPLE)
    x_deploy_qt_for_mac(${target})
  elseif(LINUX)
    x_deploy_qt_for_linux(${target})
  elseif(UNIX)
    x_deploy_qt_for_unix(${target})
  endif()
endfunction()
