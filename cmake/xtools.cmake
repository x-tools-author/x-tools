# --------------------------------------------------------------------------------------------------
# Add executable. It can be used by Qt5 and Qt6.
function(x_tools_add_executable target)
  # ARGN: all unnamed arguments
  if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${target} MANUAL_FINALIZATION ${ARGN})
  else()
    if(ANDROID)
      add_library(${target} SHARED ${ARGN})
    else()
      add_executable(${target} ${ARGN})
    endif()
  endif()

  set(macos_version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
  set(android_dir ${CMAKE_CURRENT_SOURCE_DIR}/res/android/6.8)
  set_target_properties(xTools PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
  set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER com.xtools.xtools)
  set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
  set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING ${macos_version})
  set_target_properties(xTools PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${android_dir})
  set_target_properties(xTools PROPERTIES QT_ANDROID_PACKAGE_NAME com.xtools.xtools)
endfunction()

# --------------------------------------------------------------------------------------------------
# Generate zip file
function(x_tools_generate_zip target version)
  if(WIN32)
    string(TOLOWER ${target} lower_target)
    string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} lower_system_name)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} lower_system_processor)

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${target}-zip
                    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../)

    add_custom_target(
      ${target}_zip
      COMMAND ${CMAKE_COMMAND} -E make_directory ${target}-zip
      COMMAND ${CMAKE_COMMAND} -E tar "cfv" ${target}-zip/${X_ASSET_NAME}.zip --format=zip ${target}
      WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../"
      SOURCES cmake/xtools.cmake)
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Just for test
function(x_tools_output_env flag)
  message(STATUS "[${flag}]CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
  message(STATUS "[${flag}]CMAKE_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
  message(STATUS "[${flag}]CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
  message(STATUS "[${flag}]CMAKE_CURRENT_FUNCTION_LIST_DIR: ${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Generate translations files
function(x_tools_generate_translations target)
  if(QT_VERSION VERSION_LESS "5.6.0")
    return()
  endif()

  set(APP_TS_FILES "")
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_en.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_zh_CN.ts)

  if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if(QT_VERSION VERSION_LESS "6.2.0")
      return()
    endif()

    # cmake-format: off
    if(NOT QT_VERSION VERSION_LESS "6.7.0")
      qt_add_lupdate(SOURCE_TARGETS ${target} TS_FILES ${APP_TS_FILES} LUPDATE_TARGET ${target}_lupdate NO_GLOBAL_TARGET)
    else()
      qt_add_lupdate(${target} TS_FILES ${APP_TS_FILES})
    endif()
    # cmake-format: on

    set_source_files_properties(${APP_TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR})
    if(NOT QT_VERSION VERSION_LESS "6.7.0")
      qt_add_lrelease(TS_FILES ${APP_TS_FILES} LRELEASE_TARGET ${target}_lrelease NO_GLOBAL_TARGET)
    else()
      qt_add_lrelease(${target} TS_FILES ${APP_TS_FILES})
    endif()
  else()
    if(QT_VERSION VERSION_LESS "5.6.0")
      return()
    endif()

    # cmake-format: off
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${APP_TS_FILES} OPTIONS -no-obsolete TARGET ${target}_lupdate)
    qt5_add_translation(QM_FILES ${APP_TS_FILES})
    # cmake-format: on
  endif()

  add_custom_target(
    ${target}_lupgrade
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${target}_en.qm
            ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_en.qm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${target}_zh_CN.qm
            ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_zh_CN.qm
    DEPENDS ${target}_lrelease
    COMMENT "Generate translations for ${target}...")
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for Windows
function(x_tools_deploy_qt_for_windows target)
  if(NOT DEFINED WINDEPLOYQT_EXECUTABLE)
    set(WINDEPLOYQT_EXECUTABLE "${QT_DIR}/../../../bin/windeployqt.exe")
  endif()

  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/qml)
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
      COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime
      COMMENT "Deploy Qt for Windows..."
      VERBATIM)
  endif()

  if(MSVC AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
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
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for macOS
function(x_tools_deploy_qt_for_mac target)
  # Do nothing...
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for linux
function(x_tools_deploy_qt_for_linux target)
  # Do nothing...
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt
function(x_tools_deploy_qt target)
  if(WIN32)
    x_tools_deploy_qt_for_windows(${target})
  elseif(UNIX AND NOT APPLE)
    x_tools_deploy_qt_for_linux(${target})
  elseif(APPLE)
    x_tools_deploy_qt_for_mac(${target})
  endif()
endfunction()
