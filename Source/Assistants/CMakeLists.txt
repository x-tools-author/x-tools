set(X_TOOLS_COMMON_DIR ${CMAKE_SOURCE_DIR}/Source/Common)
set(X_TOOLS_ASSISTANT_COMMON_SOURCE "")
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsSettings.h)
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsSettings.cpp)
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsApplication.h)
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsApplication.cpp)
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsDataStructure.h)
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/Common/xToolsDataStructure.cpp)
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/CommonUI/xToolsMainWindow.h)
list(APPEND X_TOOLS_ASSISTANT_COMMON_SOURCE ${X_TOOLS_COMMON_DIR}/CommonUI/xToolsMainWindow.cpp)

macro(x_tools_add_assistant_app dir_name app_name)
  string(TOUPPER ${app_name} upper_app_name)
  option(X_TOOLS_ENABLE_TARGET_${upper_app_name} "" ON)
  if(X_TOOLS_ENABLE_TARGET_${upper_app_name})
    set(ASSISTANT_ROOT_DIR ${CMAKE_SOURCE_DIR}/Source/Assistants/${dir_name})
    file(GLOB_RECURSE ASSISTANT_SOURCES_H "${ASSISTANT_ROOT_DIR}/*.h")
    file(GLOB_RECURSE ASSISTANT_SOURCES_UI "${ASSISTANT_ROOT_DIR}/*.ui")
    file(GLOB_RECURSE ASSISTANT_SOURCES_CPP "${ASSISTANT_ROOT_DIR}/*.cpp")
    file(GLOB_RECURSE ASSISTANT_SOURCES_QRC "${ASSISTANT_ROOT_DIR}/*.qrc")
    list(APPEND ASSISTANT_SOURCES ${ASSISTANT_SOURCES_H})
    list(APPEND ASSISTANT_SOURCES ${ASSISTANT_SOURCES_UI})
    list(APPEND ASSISTANT_SOURCES ${ASSISTANT_SOURCES_CPP})
    list(APPEND ASSISTANT_SOURCES ${ASSISTANT_SOURCES_QRC})
    list(APPEND ASSISTANT_SOURCES ${ASSISTANT_OWN_SOURCE})
    list(APPEND ASSISTANT_SOURCES ${X_TOOLS_ASSISTANT_COMMON_SOURCE})
    list(APPEND ASSISTANT_SOURCES ${CMAKE_SOURCE_DIR}/xTools.qrc)
    list(APPEND ASSISTANT_SOURCES ${CMAKE_SOURCE_DIR}/Source/Common/xToolsCommon.qrc)
    if(WIN32)
      list(APPEND ASSISTANT_SOURCES ${CMAKE_SOURCE_DIR}/xTools.rc)
    endif()

    add_compile_definitions(ACSS_STATIC)
    x_tools_add_executable(${app_name} ${ASSISTANT_SOURCES})
    if(X_TOOLS_ENABLE_MODULE_STYLESHEET)
      x_tools_add_stylesheet_files(${app_name})
    endif()

    x_tools_set_target_properties(${app_name})
    x_tools_deploy_qt(${app_name})
    target_link_libraries(${app_name} PRIVATE Qt${QT_VERSION_MAJOR}::Svg ${ASSISTANT_OWN_LIBS})
  endif()
endmacro()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/CRC")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/mDNS")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Ping")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ASCII")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Base64")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Number")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/String")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/QRCode")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Broadcast")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/FileCheck")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/FileMerge")
