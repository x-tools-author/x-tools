file(
  GLOB
  MODBUS_STUDIO_SOURCES
  "modbusstudio/*.h"
  "modbusstudio/*.cc"
  "modbusstudioui/*.h"
  "modbusstudioui/*.cc"
  "modbusstudioui/*.ui")
set(APP_MODBUS_STUDIO_SOURCES
    ${MODBUS_STUDIO_SOURCES}
    ../common/common/saksettings.h
    ../common/common/saksettings.cc
    ../common/commonui/sakcommonmainwindow.h
    ../common/commonui/sakcommonmainwindow.cc
    ../common/common/sakinterface.h
    ../common/common/sakinterface.cc
    ../common/common/saktranslator.h
    ../common/common/saktranslator.cc
    main.cc
    ${CMAKE_SOURCE_DIR}/qtswissarmyknife.qrc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/QtSwissArmyKnife")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(ModbusStudio MANUAL_FINALIZATION
                    ${APP_MODBUS_STUDIO_SOURCES})
else()
  if(ANDROID)
    add_library(ModbusStudio SHARED ${APP_MODBUS_STUDIO_SOURCES})
  else()
    add_executable(ModbusStudio ${APP_MODBUS_STUDIO_SOURCES})
  endif(ANDROID)
endif()

target_link_libraries(
  ModbusStudio
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort
          Qt${QT_VERSION_MAJOR}::SerialBus Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(
  ModbusStudio
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(ModbusStudio)
endif()
