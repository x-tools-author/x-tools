include(${CMAKE_SOURCE_DIR}/.cmake/assistant.cmake)

set(APP_ASSISITANT_OWN_SOURCE "")
set(APP_ASSISITANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
add_assistant_app("Base64Assistant")
