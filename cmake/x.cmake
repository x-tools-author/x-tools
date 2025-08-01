# --------------------------------------------------------------------------------------------------
# Get the last tag.
function(x_git_get_latest_tag working_dir prefix)
  execute_process(
    COMMAND git describe --abbrev=0 --tags
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_LATEST_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  set(USING_DATE_TIME FALSE)
  if(NOT GIT_LATEST_TAG)
    set(USING_DATE_TIME TRUE)
  else()
    if(${GIT_LATEST_TAG} STREQUAL "")
      set(USING_DATE_TIME TRUE)
    elseif(${GIT_LATEST_TAG} STREQUAL "continuous")
      set(USING_DATE_TIME TRUE)
    endif()
  endif()

  if(USING_DATE_TIME)
    string(TIMESTAMP current_year "%Y")
    string(TIMESTAMP current_month "%m")
    string(TIMESTAMP current_day "%d")
    math(EXPR current_month "${current_month} + 0")
    math(EXPR current_day "${current_day} + 0")
    set(target_version "v${current_year}.${current_month}.${current_day}")
    set(GIT_LATEST_TAG ${target_version})
  endif()

  message(STATUS "[git] Latest git tag(${prefix}_LATEST_GIT_TAG): ${GIT_LATEST_TAG}")
  set(${prefix}_LATEST_GIT_TAG="${GIT_LATEST_TAG}" CACHE STRING "Latest git tag" FORCE)
  set(${prefix}_LATEST_GIT_TAG
      "${GIT_LATEST_TAG}"
      PARENT_SCOPE)
  add_compile_definitions(${prefix}_LATEST_GIT_TAG="${GIT_LATEST_TAG}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Get the last commit.
function(x_git_get_latest_commit working_dir prefix)
  execute_process(
    COMMAND git log -1 --pretty=%H
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "[git] Latest git commit(${prefix}_GIT_COMMIT): ${GIT_COMMIT}")
  add_compile_definitions(${prefix}_GIT_COMMIT="${GIT_COMMIT}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Get last commit time.
function(x_git_get_latest_commit_time working_dir prefix)
  execute_process(
    COMMAND git log -1 --format=%ad --date=format:%Y.%m.%d-%H:%M:%S
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "[git] Latest git commit time(${prefix}_GIT_COMMIT_TIME): ${GIT_COMMIT_TIME}")
  add_compile_definitions(${prefix}_GIT_COMMIT_TIME="${GIT_COMMIT_TIME}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Add executable. It can be used by Qt5 and Qt6.
function(x_add_executable target)
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

  option(X_USING_VS_CODE "Using Visual Studio Code" OFF)
  if(X_USING_VS_CODE)
    set_target_properties(xTools PROPERTIES MACOSX_BUNDLE TRUE)
  else()
    set_target_properties(xTools PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
  endif()

  if(NOT QT_VERSION VERSION_LESS "6.8.0")
    set(macos_version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
    set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set_target_properties(xTools PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING ${macos_version})
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Generate zip file
function(x_generate_zip target version)
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
function(x_output_env flag)
  message(STATUS "[${flag}]CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
  message(STATUS "[${flag}]CMAKE_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
  message(STATUS "[${flag}]CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
  message(STATUS "[${flag}]CMAKE_CURRENT_FUNCTION_LIST_DIR: ${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Generate translations files
function(x_generate_translations target)
  set(APP_TS_FILES "")
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_en.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_zh_CN.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_zh_TW.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_ar.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_cs.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_da.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_de.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_es.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_fa.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_fi.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_fr.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_he.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_uk.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_it.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_ja.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_ko.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_lt.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_nl.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_pl.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_pt.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_ru.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_sk.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_sl.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_sv.ts)

  if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if(QT_VERSION VERSION_LESS "6.2.0")
      message(STATUS "[Linguist] Generating translations is ignored, because Qt version tool low")
      return()
    endif()

    # cmake-format: off
    if(NOT QT_VERSION VERSION_LESS "6.7.0")
      qt_add_lupdate(SOURCE_TARGETS ${target} TS_FILES ${APP_TS_FILES} LUPDATE_TARGET ${target}_lupdate NO_GLOBAL_TARGET)
    else()
      qt_add_lupdate(${target} TS_FILES ${APP_TS_FILES})
    endif()
    # cmake-format: on

    # set(out_dir "$<TARGET_FILE_DIR:${target}>/translations")
    set(out_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations")
    set_source_files_properties(${APP_TS_FILES} PROPERTIES OUTPUT_LOCATION ${out_dir})
    if(NOT QT_VERSION VERSION_LESS "6.7.0")
      qt_add_lrelease(TS_FILES ${APP_TS_FILES} LRELEASE_TARGET ${target}_lrelease NO_GLOBAL_TARGET)
    else()
      qt_add_lrelease(${target} TS_FILES ${APP_TS_FILES})
    endif()
  else()
    # cmake-format: off
    qt5_add_translation(QM_FILES ${APP_TS_FILES})
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${APP_TS_FILES})
    # cmake-format: on
  endif()

  add_custom_target(
    ${target}_lupgrade
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${out_dir}/${target}_en.qm
            ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_en.qm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${out_dir}/${target}_zh_CN.qm
            ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_zh_CN.qm
    DEPENDS ${target}_lrelease
    COMMENT "Generate translations for ${target}...")
endfunction()

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
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for macOS
function(x_deploy_qt_for_mac target)
  # Do nothing...
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt for linux
function(x_deploy_qt_for_linux target)
  # Do nothing...
endfunction()

# --------------------------------------------------------------------------------------------------
# Deploy Qt
function(x_deploy_qt target)
  if(WIN32)
    x_deploy_qt_for_windows(${target})
  elseif(UNIX AND NOT APPLE)
    x_deploy_qt_for_linux(${target})
  elseif(APPLE)
    x_deploy_qt_for_mac(${target})
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# 3rd party libraries(Using cmake for project management)
function(x_setup_3rd_library zip_url package_name)
  # Download the package if it does not exist
  if(NOT EXISTS ${CMAKE_SOURCE_DIR}/cmake/${package_name}.zip)
    message(STATUS "Downloading ${package_name} from ${zip_url}")
    file(
      DOWNLOAD ${zip_url} ${CMAKE_SOURCE_DIR}/3rd/${package_name}.zip
      SHOW_PROGRESS
      STATUS status)
    if(NOT status EQUAL 0)
      message(FATAL_ERROR "Failed to download ${package_name} from ${package_url}")
    endif()
  endif()

  # Extract the package
  if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${package_name})
    message(STATUS "Extracting ${package_name}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar "xzf" ${CMAKE_SOURCE_DIR}/3rd/${package_name}.zip
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd
      RESULT_VARIABLE result)
    if(result)
      message(FATAL_ERROR "Failed to extract ${package_name}")
    endif()
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Install 3rd party libraries
function(x_install_3rd_library target_name dir_name)
  add_custom_target(
    ${target_name}-deploy
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${X_LIBS_DIR}/${dir_name}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${dir_name}
    COMMENT "Deploy 3rd libraries")
endfunction()
