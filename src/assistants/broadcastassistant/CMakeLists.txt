file(GLOB BROADCAST_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_BROADCAST_ASSISTANT_SOURCES
    ${BROADCAST_ASSISTANT_SOURCES} ../../common/SAKCommonDataStructure.h
    ../../common/SAKCommonDataStructure.cc main.cc)

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
