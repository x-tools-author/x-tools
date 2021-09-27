qtHaveModule(websockets){
    DEFINES+=SAK_IMPORT_MODULE_WEBSOCKET_CLIENT
}

contains(DEFINES, SAK_IMPORT_MODULE_WEBSOCKET_CLIENT){
FORMS += \
    $$PWD/SAKWebSocketClientController.ui

HEADERS += \
    $$PWD/SAKWebSocketClientController.hh \
    $$PWD/SAKWebSocketClientDebugger.hh \
    $$PWD/SAKWebSocketClientDevice.hh

SOURCES += \
    $$PWD/SAKWebSocketClientController.cc \
    $$PWD/SAKWebSocketClientDebugger.cc \
    $$PWD/SAKWebSocketClientDevice.cc

INCLUDEPATH += \
    $$PWD
}
