set(APP_ASSISITANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/src/common/common/sakcommoninterface.h
                              ${CMAKE_SOURCE_DIR}/src/common/common/sakcommoninterface.cc)
set(APP_ASSISITANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

sak_add_assistant("number" "NumberAssistant")
