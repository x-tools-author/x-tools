file(GLOB NUMBER_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_NUMBER_ASSISTANT_SOURCES
    ${NUMBER_ASSISTANT_SOURCES} ../../common/SAKInterface.h
    ../../common/SAKInterface.cc ../../common/SAKCommonInterface.h
    ../../common/SAKCommonInterface.cc main.cc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(NumberAsistant MANUAL_FINALIZATION
                    ${APP_NUMBER_ASSISTANT_SOURCES})
else()
  if(ANDROID)
    add_library(NumberAsistant SHARED ${APP_NUMBER_ASSISTANT_SOURCES})
  else()
    add_executable(NumberAsistant ${APP_NUMBER_ASSISTANT_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(NumberAsistant PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                             Qt${QT_VERSION_MAJOR}::Network)
