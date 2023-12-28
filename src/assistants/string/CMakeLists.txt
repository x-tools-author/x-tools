set(APP_ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/src/common/common/sakcommondatastructure.h
                             ${CMAKE_SOURCE_DIR}/src/common/common/sakcommondatastructure.cc)
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("string" "StringAssistant")
sak_3rd_setup_glog(StringAssistant)
