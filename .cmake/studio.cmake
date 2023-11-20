macro(add_studio_app studio_name)
  string(TOLOWER ${studio_name} lower_studio_name)
  file(
    GLOB_RECURSE
    STUDIO_SOURCES
    "${CMAKE_SOURCE_DIR}/src/${lower_studio_name}/*.h"
    "${CMAKE_SOURCE_DIR}/src/${lower_studio_name}/*.cc"
    "${CMAKE_SOURCE_DIR}/src/${lower_studio_name}/*.ui")
  set(STUDIO_APP_SOURCES
      ${STUDIO_SOURCES}
      ${CMAKE_SOURCE_DIR}/src/common/common/saksettings.h
      ${CMAKE_SOURCE_DIR}/src/common/common/saksettings.cc
      ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.h
      ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.cc
      ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.h
      ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.cc
      ${CMAKE_SOURCE_DIR}/src/common/common/saktranslator.h
      ${CMAKE_SOURCE_DIR}/src/common/common/saktranslator.cc
      ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)

  sak_add_executable(${studio_name} ${STUDIO_APP_SOURCES})
  sak_set_target_properties(${studio_name})
  sak_auto_execute_windeployqt(${studio_name})

  target_link_libraries(${studio_name} PRIVATE ${STUDIO_APP_LIBS})
endmacro()
