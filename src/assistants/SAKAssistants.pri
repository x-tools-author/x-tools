include($$PWD/crcassistant/SAKToolCRCAssistant.pri)
include($$PWD/asciiassistant/SAKToolAsciiAssistant.pri)
include($$PWD/floatassistant/SAKToolFloatAssistant.pri)
include($$PWD/stringassistant/SAKToolStringAssistant.pri)
include($$PWD/filecheckassistant/SAKToolFileCheckAssistant.pri)
include($$PWD/broadcastassistant/SAKToolBroadcastAssistant.pri)

HEADERS += \
    $$PWD/SAKToolsFactory.hh

SOURCES += \
    $$PWD/SAKToolsFactory.cc

INCLUDEPATH += \
    $$PWD
