set(APP_ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.h
                             ${CMAKE_SOURCE_DIR}/src/common/common/sakinterface.cc)
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

sak_add_assistant("number" "NumberAssistant")
sak_3rd_setup_glog(NumberAssistant)
