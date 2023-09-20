file(GLOB STRING_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_STRING_ASSISTANT_SOURCES
    ${STRING_ASSISTANT_SOURCES}
    ../../common/common/sakcommondatastructure.h
    ../../common/common/sakcommondatastructure.cc
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
  qt_add_executable(StringAssistant MANUAL_FINALIZATION
                    ${APP_STRING_ASSISTANT_SOURCES})
else()
  if(ANDROID)
    add_library(StringAssistant SHARED ${APP_STRING_ASSISTANT_SOURCES})
  else()
    add_executable(StringAssistant ${APP_STRING_ASSISTANT_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(StringAssistant PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(
  StringAssistant
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(StringAssistant)
endif()
