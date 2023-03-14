DEFINES += SAK_IMPORT_MODULE_BROADCASTASSISTANT
INCLUDEPATH += $$PWD/src

HEADERS += \
    $$PWD/src/SAKToolBroadcastAssistant.hh \
    $$PWD/src/SAKToolBroadcastThread.hh

SOURCES += \
    $$PWD/src/SAKToolBroadcastAssistant.cc \
    $$PWD/src/SAKToolBroadcastThread.cc

FORMS += \
    $$PWD/src/SAKToolBroadcastAssistant.ui
