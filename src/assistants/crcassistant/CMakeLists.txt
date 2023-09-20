file(GLOB CRC_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_CRC_ASSISTANT_SOURCES
    ${CRC_ASSISTANT_SOURCES}
    ../../common/common/sakcommoncrcinterface.h
    ../../common/common/sakcommoncrcinterface.cc
    ../../common/commonui/sakcommonmainwindow.h
    ../../common/commonui/sakcommonmainwindow.cc
    ../../common/common/sakinterface.h
    ../../common/common/sakinterface.cc
    ../../common/common/saktranslator.h
    ../../common/common/saktranslator.cc
    ../../common/common/saksettings.h
    ../../common/common/saksettings.cc
    main.cc
    ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(CRCAssistant MANUAL_FINALIZATION
                    ${APP_CRC_ASSISTANT_SOURCES})
else()
  if(ANDROID)
    add_library(CRCAssistant SHARED ${APP_CRC_ASSISTANT_SOURCES})
  else()
    add_executable(CRCAssistant ${APP_CRC_ASSISTANT_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(CRCAssistant PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(
  CRCAssistant
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(CRCAssistant)
endif()
