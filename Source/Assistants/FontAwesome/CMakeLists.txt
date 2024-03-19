file(GLOB_RECURSE APP_ASSISTANT_OWN_SOURCE "${CMAKE_SOURCE_DIR}/3rd/qt/flowlayout/*.cpp"
     "${CMAKE_SOURCE_DIR}/3rd/qt/flowlayout/*.h")
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("fontawesome" "FontAwesomeAssistant")
sak_import_fontawesome_for_target(FontAwesomeAssistant)
sak_3rd_setup_glog(FontAwesomeAssistant)
