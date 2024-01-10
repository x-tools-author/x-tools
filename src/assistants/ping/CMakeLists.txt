set(APP_ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.h
                             ${CMAKE_SOURCE_DIR}/src/common/commonui/sakcommonmainwindow.cc)
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("ping" "PingAssistant")
sak_3rd_setup_glog(PingAssistant)
