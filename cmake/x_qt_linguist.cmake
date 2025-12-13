# --------------------------------------------------------------------------------------------------
# Generate translations files
# cmake-format: off
set(X_LANGUAGES "en" "zh_CN" "zh_TW" "ar" "cs" "da" "de" "es" "fa" "fi" "fr" "he" "uk" "it" "ja" "ko" "lt" "nl" "pl" "pt" "ru" "sk" "sl" "sv")
# cmake-format: on

# Generate translations files for module
function(x_generate_module_translations module_name dir recurse)
  if(QT_VERSION VERSION_LESS "6.7.0")
    return()
  endif()

  if(IOS)
    return()
  endif()

  set(ts_files "")
  foreach(lang IN LISTS X_LANGUAGES)
    set(ts_file ${CMAKE_CURRENT_LIST_DIR}/res/translations/${module_name}/${module_name}_${lang}.ts)
    list(APPEND ts_files ${ts_file})
  endforeach()

  if(recurse)
    file(GLOB_RECURSE files ${dir}/*.h ${dir}/*.cpp ${dir}/*.ui)
  else()
    file(GLOB files ${dir}/*.h ${dir}/*.cpp ${dir}/*.ui)
  endif()

  set(out_dir "")
  if(APPLE)
    file(GLOB tmp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*.app)
    foreach(app IN LISTS tmp)
      if(IS_DIRECTORY ${app})
        set(out_dir "${app}/Contents/MacOS/translations/${module_name}")
        break()
      endif()
    endforeach()

    if(out_dir STREQUAL "")
      return()
    endif()
  endif()

  qt_add_lupdate(TS_FILES ${ts_files} LUPDATE_TARGET ${module_name}_lupdate SOURCES ${files})
  if(APPLE)
    message(STATUS "[Linguist]The translations will be installed to: ${out_dir}")
    set_source_files_properties(${ts_files} PROPERTIES OUTPUT_LOCATION ${out_dir})
  else()
    set(out_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations/${module_name}")
    set_source_files_properties(${ts_files} PROPERTIES OUTPUT_LOCATION ${out_dir})
  endif()

  qt_add_lrelease(TS_FILES ${ts_files} LRELEASE_TARGET ${module_name}_lrelease)
  set_property(TARGET ${module_name}_lupdate PROPERTY FOLDER "i18n")
  set_property(TARGET ${module_name}_lrelease PROPERTY FOLDER "i18n")
  set_property(TARGET update_translations PROPERTY FOLDER "i18n")
  set_property(TARGET release_translations PROPERTY FOLDER "i18n")
endfunction()

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
    if(APPLE)
      set(out_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}.app/Contents/MacOS/translations")
    else()
      set(out_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations")
    endif()
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

  # * add_custom_target(
  # * ${target}_lupgrade
  # * COMMAND ${CMAKE_COMMAND} -E copy_if_different ${out_dir}/${target}_en.qm
  # * ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_en.qm
  # * COMMAND ${CMAKE_COMMAND} -E copy_if_different ${out_dir}/${target}_zh_CN.qm
  # * ${CMAKE_CURRENT_LIST_DIR}/res/translations/${target}_zh_CN.qm
  # * DEPENDS ${target}_lrelease
  # * COMMENT "Generate translations for ${target}...")
endfunction()
