file(GLOB FILECHECK_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_FILECHECK_ASSISTANT_SOURCES ${FILECHECK_ASSISTANT_SOURCES} main.cc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(FileCheckAssistant MANUAL_FINALIZATION
                    ${APP_FILECHECK_ASSISTANT_SOURCES})
else()
  if(ANDROID)
    add_library(FileCheckAssistant SHARED ${APP_FILECHECK_ASSISTANT_SOURCES})
  else()
    add_executable(FileCheckAssistant ${APP_FILECHECK_ASSISTANT_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(FileCheckAssistant PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
