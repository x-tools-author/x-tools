macro(x_tools_add_assistant dir_name app_name)
  string(TOUPPER ${app_name} upper_app_name)
  option(X_TOOLS_ENABLE_APP_${upper_app_name} "" ON)
  if(X_TOOLS_ENABLE_APP_${upper_app_name})
    file(
      GLOB_RECURSE
      ASSISTANT_SOURCES
      "${CMAKE_SOURCE_DIR}/Source/Assistants/${dir_name}/*.h"
      "${CMAKE_SOURCE_DIR}/Source/Assistants/${dir_name}/*.cpp"
      "${CMAKE_SOURCE_DIR}/Source/Assistants/${dir_name}/*.ui"
      "${CMAKE_SOURCE_DIR}/Source/Assistants/${dir_name}/*.qrc")
    set(APP_ASSISTANT_SOURCES
        ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.h
        ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsInterface.cpp
        ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.h
        ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsTranslator.cpp
        ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.h
        ${CMAKE_SOURCE_DIR}/Source/Common/Common/xToolsSettings.cpp
        ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsMainWindow.h
        ${CMAKE_SOURCE_DIR}/Source/Common/CommonUI/xToolsMainWindow.cpp
        ${APP_ASSISTANT_OWN_SOURCE}
        ${ASSISTANT_SOURCES}
        ${CMAKE_SOURCE_DIR}/xTools.qrc)

    if(WIN32)
      list(APPEND APP_ASSISTANT_SOURCES ${CMAKE_SOURCE_DIR}/xTools.rc)
    endif()

    sak_add_executable(${app_name} ${APP_ASSISTANT_SOURCES})
    sak_auto_execute_deployqt(${app_name})
    sak_set_target_properties(${app_name})
    target_link_libraries(${app_name} PRIVATE ${APP_ASSISTANT_OWN_LIBS})
  endif()
endmacro()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CRC)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mDNS)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Ping)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ASCII)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Base64)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Number)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/String)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Broadcast)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/FileCheck)
