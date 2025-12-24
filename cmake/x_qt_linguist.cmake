set(X_LANGUAGES
    "en"
    "zh_CN"
    "zh_TW"
    "ar"
    "cs"
    "da"
    "de"
    "es"
    "fa"
    "fi"
    "fr"
    "he"
    "uk"
    "it"
    "ja"
    "ko"
    "lt"
    "nl"
    "pl"
    "pt"
    "ru"
    "sk"
    "sl"
    "sv")

# Use xTools translations
function(x_using_x_tools_translations target)
  file(GLOB_RECURSE ts_files ${CMAKE_SOURCE_DIR}/res/translations/*.ts)
  set(out_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations")
  set_source_files_properties(${ts_files} PROPERTIES OUTPUT_LOCATION ${out_dir})
  qt_add_lrelease(TS_FILES ${ts_files} LRELEASE_TARGET ${target}_lrelease)
  set_property(TARGET release_translations PROPERTY FOLDER "i18n")
  set_property(TARGET ${target}_lrelease PROPERTY FOLDER "i18n")
endfunction()

# Generate translations files for application
function(x_generate_translations target)
  # Do not generate translations for mobile platforms
  if(IOS OR ANDROID)
    return()
  endif()

  set(APP_TS_FILES "")
  foreach(lang IN LISTS X_LANGUAGES)
    set(ts_file ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_${lang}.ts)
    list(APPEND APP_TS_FILES ${ts_file})
  endforeach()
  set(out_dir "${CMAKE_BINARY_DIR}/x_tmp/${target}/translations")

  # Qt5
  if(${QT_VERSION} VERSION_LESS "6.0.0")
    # * Do not use qt5_create_translation, I do not want to update ts files automatically.
    # * qt5_create_translation(QM_FILES ${APP_TS_FILES})

    # ts -> qm
    qt5_add_translation(QM_FILES ${APP_TS_FILES})
    add_custom_target(${target}_lrelease DEPENDS ${QM_FILES} SOURCE ${CMAKE_CURRENT_LIST_FILE})
    add_dependencies(${target} ${target}_lrelease)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} ${out_dir}
      COMMENT "Collect Qt5 translation QMs for ${target}...")
    set(LUPDATE_EXECUTABLE "${QT_DIR}/../../../bin/lupdate.exe")
    # lupdate ts files(just for qt5)
    if(EXISTS ${LUPDATE_EXECUTABLE})
      list(APPEND args -DargTarget=${target})
      list(APPEND args -DargLupdate=${LUPDATE_EXECUTABLE})
      list(APPEND args -DargSrcDir=${CMAKE_SOURCE_DIR}/src)
      list(APPEND args -DargOutDir=${CMAKE_SOURCE_DIR}/res/translations)
      set(cmake_script ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/x_qt_linguist_script.cmake)
      add_custom_target(
        ${target}_lupdate
        COMMAND ${CMAKE_COMMAND} ${args} -P ${cmake_script}
        SOURCES ${CMAKE_CURRENT_LIST_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
        COMMENT "Start update ts files for ${target}, the args are: ${args}")
    endif()
  else()
    set_source_files_properties(${APP_TS_FILES} PROPERTIES OUTPUT_LOCATION ${out_dir})
    if(NOT QT_VERSION VERSION_LESS "6.7.0")
      qt_add_lupdate(
        SOURCE_TARGETS
        ${target}
        TS_FILES
        ${APP_TS_FILES}
        LUPDATE_TARGET
        ${target}_lupdate
        NO_GLOBAL_TARGET)
      qt_add_lrelease(TS_FILES ${APP_TS_FILES} LRELEASE_TARGET ${target}_lrelease NO_GLOBAL_TARGET)
    else()
      qt_add_lupdate(${target} TS_FILES ${APP_TS_FILES})
      qt_add_lrelease(${target} TS_FILES ${APP_TS_FILES})
    endif()
  endif()

  # Copy qm files to deploy dir...
  if(APPLE)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND
        ${CMAKE_COMMAND} -E copy_directory_if_different ${out_dir}
        "$<TARGET_BUNDLE_DIR:${target}>/Contents/Resources/translations/" "||" ${CMAKE_COMMAND} -E
        true
      COMMENT "Generate translations for ${target}...")
  else()
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${out_dir}
              "$<TARGET_FILE_DIR:${target}>/translations/" "||" ${CMAKE_COMMAND} -E true
      COMMENT "Generate translations for ${target}...")
  endif()
endfunction()
