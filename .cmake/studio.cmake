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

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
  if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${studio_name} MANUAL_FINALIZATION ${STUDIO_APP_SOURCES})
  else()
    if(ANDROID)
      add_library(${studio_name} SHARED ${STUDIO_APP_SOURCES})
    else()
      add_executable(${studio_name} ${STUDIO_APP_SOURCES})
    endif(ANDROID)
  endif()

  target_link_libraries(${studio_name} PRIVATE ${STUDIO_APP_LIBS})
  set_target_properties(
    ${studio_name}
    PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
               ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
               TRUE WIN32_EXECUTABLE
               TRUE)

  if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${studio_name})
  endif()
endmacro()
