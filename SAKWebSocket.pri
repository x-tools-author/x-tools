qtHaveModule(websockets){
    QT  += websockets
    DEFINES+=SAK_IMPORT_MODULE_WEBSOCKET
}

contains(DEFINES, SAK_IMPORT_MODULE_WEBSOCKET){
    INCLUDEPATH += \
        $$PWD/src/debuggers/websocket/client \
        $$PWD/src/debuggers/websocket/server

    FORMS += \
        $$PWD/src/debuggers/websocket/client/SAKWebSocketClientDeviceController.ui \
        $$PWD/src/debuggers/websocket/server/SAKWebSocketServerDeviceController.ui

    HEADERS += \
        $$PWD/src/debuggers/websocket/client/SAKWebSocketClientDebugPage.hh \
        $$PWD/src/debuggers/websocket/client/SAKWebSocketClientDevice.hh \
        $$PWD/src/debuggers/websocket/client/SAKWebSocketClientDeviceController.hh \
        $$PWD/src/debuggers/websocket/server/SAKWebSocketServerDebugPage.hh \
        $$PWD/src/debuggers/websocket/server/SAKWebSocketServerDevice.hh \
        $$PWD/src/debuggers/websocket/server/SAKWebSocketServerDeviceController.hh

    SOURCES += \
        $$PWD/src/debuggers/websocket/client/SAKWebSocketClientDebugPage.cc \
        $$PWD/src/debuggers/websocket/client/SAKWebSocketClientDevice.cc \
        $$PWD/src/debuggers/websocket/client/SAKWebSocketClientDeviceController.cc \
        $$PWD/src/debuggers/websocket/server/SAKWebSocketServerDebugPage.cc \
        $$PWD/src/debuggers/websocket/server/SAKWebSocketServerDevice.cc \
        $$PWD/src/debuggers/websocket/server/SAKWebSocketServerDeviceController.cc
}else {
    message("The Qt edition has no websocket module, the program will not has websocket debugging function.")
}
