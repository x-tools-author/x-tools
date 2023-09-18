file(GLOB NUMBER_ASSISTANT_SOURCES "src/*.h" "src/*.cc" "src/*.ui")
set(APP_NUMBER_ASSISTANT_SOURCES
    ${NUMBER_ASSISTANT_SOURCES} ../../common/common/SAKInterface.h
    ../../common/common/SAKInterface.cc ../../common/common/SAKCommonInterface.h
    ../../common/common/SAKCommonInterface.cc main.cc)

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

set_target_properties(
  NumberAsistant
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(NumberAsistant)
endif()
