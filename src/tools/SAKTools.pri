#include($$PWD/atassistant/SAKAtAssistant.pri)
include($$PWD/filechecker/SAKToolFileChecker.pri)
include($$PWD/crccalculator/SAKToolCRCCalculator.pri)
include($$PWD/asciiassistant/SAKToolAsciiAssistant.pri)
include($$PWD/floatassistant/SAKToolFloatAssistant.pri)
include($$PWD/stringassistant/SAKToolStringAssistant.pri)

HEADERS += \
    $$PWD/SAKToolsFactory.hh

SOURCES += \
    $$PWD/SAKToolsFactory.cc

INCLUDEPATH += \
    $$PWD
