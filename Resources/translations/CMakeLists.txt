set(lupdate "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/lupdate.exe")
set(lrelease "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/lrelease.exe")

function(sak_linguist_update)
  # Create ts file.
  set(COMMON_STR ${lupdate} ${CMAKE_SOURCE_DIR} -no-obsolete -ts)
  add_custom_target(
    LinguistUpdate
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_en.ts
    COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_zh_CN.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ar.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_cs.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_da.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_de.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_es.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_fa.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_fi.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_fr.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_he.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_uk.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_it.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ja.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ko.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_lt.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_pl.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_pt.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_ru.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_sk.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_sl.ts
    # COMMAND ${COMMON_STR} ${CMAKE_SOURCE_DIR}/resources/translations/sak_sv.ts
    SOURCES ${CMAKE_SOURCE_DIR}/.cmake/sak_i18n.cmake
    VERBATIM)
endfunction()

function(sak_linguist_release)
  # Create qm file.
  add_custom_target(
    LinguistRelease
    COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_en.ts
    COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_zh_CN.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ar.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_cs.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_da.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_de.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_es.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_fa.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_fi.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_fr.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_he.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_uk.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_it.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ja.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ko.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_lt.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_pl.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_pt.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_ru.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_sk.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_sl.ts
    # COMMAND ${lrelease} -compress ${CMAKE_SOURCE_DIR}/resources/translations/sak_sv.ts
    SOURCES ${CMAKE_SOURCE_DIR}/.cmake/sak_i18n.cmake
    VERBATIM)

  add_dependencies(LinguistRelease LinguistUpdate)

  # Auto create ts files and qm files.
  option(SAK_AUTO_I18N "" OFF)
  if(SAK_AUTO_I18N)
    add_custom_target(sak_i18n)
    add_dependencies(sak_i18n LinguistRelease)
  endif()
endfunction()

sak_linguist_update()
sak_linguist_release()
