function(sak_add_lupdate)
  set(LUPDATE "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/lupdate.exe")
  set(LRELEASE "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/lrelease.exe")

  # Create ts file.
  set(COMMON_STR ${LUPDATE} ${CMAKE_SOURCE_DIR} -no-obsolete -ts)
  add_custom_target(
    sak_lupdate
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_en.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_zh_CN.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_ar.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_cs.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_da.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_de.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_es.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_fa.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_fi.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_fr.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_he.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_uk.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_it.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_ja.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_ko.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_lt.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_pl.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_pt.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_ru.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_sk.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_sl.ts
    COMMAND ${COMMON_STR} ${CMAKE_CURRENT_SOURCE_DIR}/sak_sv.ts
    VERBATIM)
endfunction()

function(sak_add_lrelease)
  # Create qm file.
  add_custom_target(
    sak_lrelease
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_en.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_zh_CN.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_ar.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_cs.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_da.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_de.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_es.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_fa.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_fi.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_fr.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_he.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_uk.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_it.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_ja.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_ko.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_lt.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_pl.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_pt.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_ru.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_sk.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_sl.ts
    COMMAND ${LRELEASE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/sak_sv.ts
    VERBATIM)

  add_dependencies(sak_lrelease sak_lupdate)

  # Auto create ts files and qm files.
  option(SAK_AUTO_I18N "" OFF)
  if(SAK_AUTO_I18N)
    add_custom_target(sak_i18n ALL)
    add_dependencies(sak_i18n sak_lrelease)
  endif()
endfunction()
