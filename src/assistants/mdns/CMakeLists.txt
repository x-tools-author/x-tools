set(APP_ASSISITANT_OWN_SOURCE "")
set(APP_ASSISITANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("mdns" "MDNSAssistant")
sak_import_fontawesome_for_target(MDNSAssistant)
