set(APP_ASSISTANT_OWN_SOURCE "")
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("ascii" "AsciiAssistant")
sak_3rd_setup_glog(AsciiAssistant)
