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

set_target_properties(
  FileCheckAssistant
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(FileCheckAssistant)
endif()
