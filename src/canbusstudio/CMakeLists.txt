include(${CMAKE_SOURCE_DIR}/.cmake/studio.cmake)

set(STUDIO_APP_LIBS Qt${QT_VERSION_MAJOR}::Widgets
                    Qt${QT_VERSION_MAJOR}::SerialBus)
add_studio_app(CANBusStudio)
