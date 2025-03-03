# --------------------------------------------------------------------------------------------------
# Add executable. It can be used by Qt5 and Qt6.
function(x_tools_add_executable target)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}")
  if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${target} MANUAL_FINALIZATION)
    if(${target} STREQUAL "xTools")
      set_property(
        TARGET ${target}
        APPEND
        PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
    endif()
  else()
    if(ANDROID)
      add_library(${target} SHARED ${ARGS})
    else()
      add_executable(${target} ${ARGS})
    endif()
  endif()

  set(INDEX 0)
  while(INDEX LESS ${ARGC})
    math(EXPR INDEX "${INDEX} + 1")
    target_sources(${target} PRIVATE ${ARGV${INDEX}})
  endwhile()

  if(ANDROID)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${target}>"
              "${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/$<TARGET_FILE_NAME:${target}>")
  endif()

  set_target_properties(
    ${target}
    PROPERTIES # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled3
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)
endfunction()

# --------------------------------------------------------------------------------------------------
# Generate zip file
function(x_tools_generate_zip target version)
  if(WIN32)
    string(TOLOWER ${target} lower_target)
    string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} lower_system_name)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} lower_system_processor)
    set(TAR_FILE_NAME ${lower_target}-${lower_system_name}-${lower_system_processor}-v${version})

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${target}-zip
                    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../)

    add_custom_target(
      ${target}_zip
      COMMAND ${CMAKE_COMMAND} -E tar "cfz" ${target}-zip/${TAR_FILE_NAME}.zip ${target}
      WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../"
      SOURCES cmake/xtools.cmake)
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Generate translations files
function(x_tools_generate_translations target)
  if(QT_VERSION VERSION_LESS "6.2.0")
    return()
  endif()

  set(APP_TS_FILES "")
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_en.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_zh_CN.ts)
  # cmake-format: off
  if(NOT QT_VERSION VERSION_LESS "6.7.0")
    qt_add_lupdate(SOURCE_TARGETS ${target} TS_FILES ${APP_TS_FILES} LUPDATE_TARGET ${target}_lupdate NO_GLOBAL_TARGET)
  else()
    set(O_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/translations")
    qt_add_lupdate(${target} TS_FILES ${APP_TS_FILES})
  endif()
  # cmake-format: on

  set_source_files_properties(${APP_TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR})
  if(NOT QT_VERSION VERSION_LESS "6.7.0")
    qt_add_lrelease(TS_FILES ${APP_TS_FILES} LRELEASE_TARGET ${target}_lrelease NO_GLOBAL_TARGET)
  else()
    qt_add_lrelease(${target} TS_FILES ${APP_TS_FILES})
  endif()

  add_custom_target(
    ${target}_lupgrade
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${target}_en.qm
            ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_en.qm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${target}_zh_CN.qm
            ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_zh_CN.qm
    DEPENDS ${target}_lrelease
    COMMENT "Generate translations for ${target}...")
endfunction()

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
