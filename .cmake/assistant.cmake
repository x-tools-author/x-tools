macro(add_assistant_app app_name)
  string(TOLOWER ${app_name} lower_app_name)
  string(TOUPPER ${app_name} upper_app_name)
  option(APP_${upper_app_name} "" ON)
  if(APP_${upper_app_name})
    file(
      GLOB_RECURSE
      ASSISTANT_SOURCES
      "${CMAKE_SOURCE_DIR}/src/assistants/${lower_app_name}/*.h"
      "${CMAKE_SOURCE_DIR}/src/assistants/${lower_app_name}/*.cc"
      "${CMAKE_SOURCE_DIR}/src/assistants/${lower_app_name}/*.ui"
      "${CMAKE_SOURCE_DIR}/src/assistants/${lower_app_name}/*.qrc")
    set(APP_ASSISTANT_SOURCES
        ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.h
        ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.cc
        ${CMAKE_SOURCE_DIR}/src/common/common/saktranslator.h
        ${CMAKE_SOURCE_DIR}/src/common/common/saktranslator.cc
        ${CMAKE_SOURCE_DIR}/src/common/common/saksettings.h
        ${CMAKE_SOURCE_DIR}/src/common/common/saksettings.cc
        ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.h
        ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.cc
        ${APP_ASSISITANT_OWN_SOURCE}
        ${ASSISTANT_SOURCES}
        ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)
    sak_add_executable(${app_name} ${APP_ASSISTANT_SOURCES})
    sak_auto_execute_deployqt(${app_name})
    target_link_libraries(${app_name} PRIVATE ${APP_ASSISITANT_OWN_LIBS})
    set_target_properties(
      ${app_name}
      PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
                 ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
                 ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
                 TRUE WIN32_EXECUTABLE
                 TRUE)

    if(QT_VERSION_MAJOR EQUAL 6)
      qt_finalize_executable(${app_name})
    endif()
  endif(APP_${upper_app_name})
endmacro()
