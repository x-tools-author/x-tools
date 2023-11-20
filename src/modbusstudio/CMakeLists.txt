include(${CMAKE_SOURCE_DIR}/.cmake/studio.cmake)

set(STUDIO_APP_LIBS
    Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialBus
    Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Network)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/sak/ModbusStudio")
add_studio_app(ModbusStudio)
