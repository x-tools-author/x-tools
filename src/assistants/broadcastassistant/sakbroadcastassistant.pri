DEFINES += SAK_IMPORT_MODULE_BROADCASTASSISTANT
INCLUDEPATH += $$PWD/src

HEADERS += \
    $$PWD/src/SAKToolBroadcastAssistant.h \
    $$PWD/src/SAKToolBroadcastThread.h

SOURCES += \
    $$PWD/src/SAKToolBroadcastAssistant.cc \
    $$PWD/src/SAKToolBroadcastThread.cc

FORMS += \
    $$PWD/src/SAKToolBroadcastAssistant.ui
