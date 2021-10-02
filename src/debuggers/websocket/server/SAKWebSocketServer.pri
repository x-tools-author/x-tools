qtHaveModule(websockets){
    QT  += websockets
    DEFINES+=SAK_IMPORT_MODULE_WEBSOCKET_SERVER
}

contains(DEFINES, SAK_IMPORT_MODULE_WEBSOCKET_SERVER){
FORMS += \
    $$PWD/SAKWebSocketServerController.ui

HEADERS += \
    $$PWD/SAKWebSocketServerController.hh \
    $$PWD/SAKWebSocketServerDebugger.hh \
    $$PWD/SAKWebSocketServerDevice.hh

SOURCES += \
    $$PWD/SAKWebSocketServerController.cc \
    $$PWD/SAKWebSocketServerDebugger.cc \
    $$PWD/SAKWebSocketServerDevice.cc

INCLUDEPATH += \
    $$PWD
}
