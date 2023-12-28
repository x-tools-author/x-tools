set(APP_ASSISTANT_OWN_SOURCE "")
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("mdns" "MDNSAssistant")

sak_3rd_setup_glog(MDNSAssistant)
sak_3rd_setup_qmdnsengine(MDNSAssistant)
