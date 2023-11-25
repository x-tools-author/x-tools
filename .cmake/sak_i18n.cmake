function(sak_add_lupdate)
  set(LUPDATE "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/lupdate.exe")
  set(LRELEASE "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/lrelease.exe")

  # Create ts file.
  set(COMMON_STR ${LUPDATE} ${CMAKE_SOURCE_DIR} -no-obsolete -ts)
  add_custom_target(
    sak_lupdate
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_en.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_zh_CN.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ar.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_cs.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_da.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_de.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_es.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_fa.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_fi.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_fr.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_he.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_uk.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_it.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ja.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ko.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_lt.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_pl.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_pt.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ru.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_sk.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_sl.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_sv.ts
    VERBATIM)
endfunction()

function(sak_add_lrelease)
  # Create qm file.
  add_custom_target(
    sak_lrelease
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_en.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_zh_CN.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ar.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_cs.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_da.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_de.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_es.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_fa.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_fi.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_fr.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_he.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_uk.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_it.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ja.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ko.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_lt.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_pl.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_pt.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ru.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_sk.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_sl.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_sv.ts
    VERBATIM)

  add_dependencies(sak_lrelease sak_lupdate)

  # Auto create ts files and qm files.
  option(SAK_AUTO_I18N "" OFF)
  if(SAK_AUTO_I18N)
    add_custom_target(sak_i18n ALL)
    add_dependencies(sak_i18n sak_lrelease)
  endif()
endfunction()
