file(
  GLOB
  CANBUS_STUDIO_SOURCES
  "canbus/*.h"
  "canbus/*.cc"
  "canbusui/*.h"
  "canbusui/*.cc"
  "canbusui/*.ui")
set(APP_CANBUS_STUDIO_SOURCES ${CANBUS_STUDIO_SOURCES} ../common/SAKSettings.h
                              ../common/SAKSettings.cc main.cc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(CANBusStudio MANUAL_FINALIZATION
                    ${APP_CANBUS_STUDIO_SOURCES})
else()
  if(ANDROID)
    add_library(CANBusStudio SHARED ${APP_CANBUS_STUDIO_SOURCES})
  else()
    add_executable(CANBusStudio ${APP_CANBUS_STUDIO_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(CANBusStudio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                           Qt${QT_VERSION_MAJOR}::SerialBus)

set_target_properties(
  CANBusStudio
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(CANBusStudio)
endif()
