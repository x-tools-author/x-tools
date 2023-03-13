#include($$PWD/atassistant/SAKAtAssistant.pri)
include($$PWD/crccalculator/SAKToolCRCCalculator.pri)
include($$PWD/asciiassistant/SAKToolAsciiAssistant.pri)
include($$PWD/floatassistant/SAKToolFloatAssistant.pri)
include($$PWD/stringassistant/SAKToolStringAssistant.pri)
include($$PWD/filecheckassistant/SAKToolFileCheckAssistant.pri)

HEADERS += \
    $$PWD/SAKToolsFactory.hh

SOURCES += \
    $$PWD/SAKToolsFactory.cc

INCLUDEPATH += \
    $$PWD
