QT  += serialport

DEFINES += SAK_IMPORT_MODULE_ATASSISTANT
INCLUDEPATH += $$PWD/src/atassistant

HEADERS += \
    $$PWD/src/atassistant/SAKAtAssistant.hh

SOURCES += \
    $$PWD/src/atassistant/SAKAtAssistant.cc

FORMS += \
    $$PWD/src/atassistant/SAKAtAssistant.ui
