set(APP_ASSISTANT_OWN_SOURCE "")
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("base64" "Base64Assistant")
sak_3rd_setup_glog(Base64Assistant)
