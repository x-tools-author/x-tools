macro(sak_add_assistant dir_name app_name)
  string(TOUPPER ${app_name} upper_app_name)
  option(SAK_ENABLE_APP_${upper_app_name} "" ON)
  if(SAK_ENABLE_APP_${upper_app_name})
    file(
      GLOB_RECURSE
      ASSISTANT_SOURCES
      "${CMAKE_SOURCE_DIR}/src/assistants/${dir_name}/*.h"
      "${CMAKE_SOURCE_DIR}/src/assistants/${dir_name}/*.cc"
      "${CMAKE_SOURCE_DIR}/src/assistants/${dir_name}/*.ui"
      "${CMAKE_SOURCE_DIR}/src/assistants/${dir_name}/*.qrc")
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
    sak_set_target_properties(${app_name})
    target_link_libraries(${app_name} PRIVATE ${APP_ASSISITANT_OWN_LIBS})
  endif()
endmacro()

add_subdirectory(crc)
add_subdirectory(ascii)
add_subdirectory(base64)
add_subdirectory(number)
add_subdirectory(string)
add_subdirectory(broadcast)
add_subdirectory(filecheck)
