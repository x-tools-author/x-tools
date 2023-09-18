file(GLOB ASCII_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_ASCII_ASSISTANT_SOURCES ${ASCII_ASSISTANT_SOURCES} main.cc
                                SAKToolAsciiAssistant.qrc)

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
