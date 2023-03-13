#include($$PWD/atassistant/SAKAtAssistant.pri)
include($$PWD/crcassistant/SAKToolCRCAssistant.pri)
include($$PWD/asciiassistant/SAKToolAsciiAssistant.pri)
include($$PWD/floatassistant/SAKToolFloatAssistant.pri)
#include($$PWD/qrcodeassistant/SAKToolQRCodeAssistant.pri)
include($$PWD/stringassistant/SAKToolStringAssistant.pri)
include($$PWD/filecheckassistant/SAKToolFileCheckAssistant.pri)

HEADERS += \
    $$PWD/SAKToolsFactory.hh

SOURCES += \
    $$PWD/SAKToolsFactory.cc

INCLUDEPATH += \
    $$PWD
