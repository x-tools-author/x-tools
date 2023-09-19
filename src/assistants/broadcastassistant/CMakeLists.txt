file(GLOB BROADCAST_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_BROADCAST_ASSISTANT_SOURCES
    ${BROADCAST_ASSISTANT_SOURCES}
    ../../common/common/SAKCommonDataStructure.h
    ../../common/common/SAKCommonDataStructure.cc
    ../../common/commonui/sakcommonmainwindow.h
    ../../common/commonui/sakcommonmainwindow.cc
    ../../common/common/SAKInterface.h
    ../../common/common/SAKInterface.cc
    ../../common/common/SAKTranslator.h
    ../../common/common/SAKTranslator.cc
    ../../common/common/SAKSettings.h
    ../../common/common/SAKSettings.cc
    ../common/sakassistantcommon.h
    main.cc
    ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)

include_directories(../common)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(BroadcastAssistant MANUAL_FINALIZATION
                    ${APP_BROADCAST_ASSISTANT_SOURCES})
else()
  if(ANDROID)
    add_library(BroadcastAssistant SHARED ${APP_BROADCAST_ASSISTANT_SOURCES})
  else()
    add_executable(BroadcastAssistant ${APP_BROADCAST_ASSISTANT_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(BroadcastAssistant PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                 Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(
  BroadcastAssistant
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(BroadcastAssistant)
endif()
