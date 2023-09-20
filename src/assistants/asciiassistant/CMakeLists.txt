file(GLOB ASCII_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_ASCII_ASSISTANT_SOURCES
    ${ASCII_ASSISTANT_SOURCES}
    ../../common/commonui/sakcommonmainwindow.h
    ../../common/commonui/sakcommonmainwindow.cc
    ../../common/common/sakinterface.h
    ../../common/common/sakinterface.cc
    ../../common/common/saktranslator.h
    ../../common/common/saktranslator.cc
    ../../common/common/saksettings.h
    ../../common/common/saksettings.cc
    main.cc
    sakasciiassistant.qrc
    ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(ASCIIAssistant MANUAL_FINALIZATION
                    ${APP_ASCII_ASSISTANT_SOURCES})
else()
  if(ANDROID)
    add_library(ASCIIAssistant SHARED ${APP_ASCII_ASSISTANT_SOURCES})
  else()
    add_executable(ASCIIAssistant ${APP_ASCII_ASSISTANT_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(ASCIIAssistant PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(
  ASCIIAssistant
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(ASCIIAssistant)
endif()
