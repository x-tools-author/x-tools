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
  set(APP_TS_FILES "")
  foreach(lang IN LISTS X_LANGUAGES)
    set(ts_file ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_${lang}.ts)
    list(APPEND APP_TS_FILES ${ts_file})
  endforeach()

  # Qt5
  if(${QT_VERSION} VERSION_LESS "6.0.0")
    qt5_add_translation(QM_FILES ${APP_TS_FILES})
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${APP_TS_FILES})
    return()
  endif()

  if(QT_VERSION VERSION_LESS "6.2.0")
    message(STATUS "[Linguist] Generating translations is ignored, because Qt version tool low")
    return()
  endif()

  # Do not generate translations for mobile platforms
  if(IOS OR ANDROID)
    return()
  endif()

  set(out_dir "${CMAKE_BINARY_DIR}/x_tmp/${target}/translations")
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
              "$<TARGET_FILE_DIR:${target}>/translations/"
      COMMENT "Generate translations for ${target}...")
  endif()
endfunction()
