include(${CMAKE_SOURCE_DIR}/.cmake/studio.cmake)

set(STUDIO_APP_LIBS Qt${QT_VERSION_MAJOR}::Widgets
                    Qt${QT_VERSION_MAJOR}::SerialBus)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/sak/CANBusStudio")
add_studio_app(CANBusStudio)
