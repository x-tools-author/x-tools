set(APP_ASSISTANT_OWN_SOURCE ${CMAKE_SOURCE_DIR}/src/common/common/sakcommoncrcinterface.h
                             ${CMAKE_SOURCE_DIR}/src/common/common/sakcommoncrcinterface.cc)
set(APP_ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

sak_add_assistant("crc" "CrcAssistant")
