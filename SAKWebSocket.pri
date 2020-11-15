qtHaveModule(websockets){
    QT  += websockets
    DEFINES+=SAK_IMPORT_MODULE_WEBSOCKET
}

contains(DEFINES, SAK_IMPORT_MODULE_WEBSOCKET){
    INCLUDEPATH += \
        $$PWD/src/pages/websocket/client \
        $$PWD/src/pages/websocket/server

    FORMS += \
        $$PWD/src/pages/websocket/client/SAKWebSocketClientDeviceController.ui \
        $$PWD/src/pages/websocket/server/SAKWebSocketServerDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/websocket/client/SAKWebSocketClientDebugPage.hh \
        $$PWD/src/pages/websocket/client/SAKWebSocketClientDevice.hh \
        $$PWD/src/pages/websocket/client/SAKWebSocketClientDeviceController.hh \
        $$PWD/src/pages/websocket/server/SAKWebSocketServerDebugPage.hh \
        $$PWD/src/pages/websocket/server/SAKWebSocketServerDevice.hh \
        $$PWD/src/pages/websocket/server/SAKWebSocketServerDeviceController.hh

    SOURCES += \
        $$PWD/src/pages/websocket/client/SAKWebSocketClientDebugPage.cc \
        $$PWD/src/pages/websocket/client/SAKWebSocketClientDevice.cc \
        $$PWD/src/pages/websocket/client/SAKWebSocketClientDeviceController.cc \
        $$PWD/src/pages/websocket/server/SAKWebSocketServerDebugPage.cc \
        $$PWD/src/pages/websocket/server/SAKWebSocketServerDevice.cc \
        $$PWD/src/pages/websocket/server/SAKWebSocketServerDeviceController.cc
}else {
    message("The Qt edition has no websocket module, the program will not has websocket debugging function.")
}
