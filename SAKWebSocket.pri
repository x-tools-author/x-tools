qtHaveModule(websockets){
    QT  += websockets
    DEFINES+=SAK_IMPORT_WEBSOCKET_MODULE
}

contains(DEFINES, SAK_IMPORT_WEBSOCKET_MODULE){
    FORMS += \
    src/pages/websocketclient/SAKWebSocketClientDeviceController.ui \
    src/pages/websocketserver/SAKWebSocketServerDeviceController.ui

    HEADERS += \
        src/pages/websocketclient/SAKWebSocketClientDebugPage.hh \
        src/pages/websocketclient/SAKWebSocketClientDevice.hh \
        src/pages/websocketclient/SAKWebSocketClientDeviceController.hh \
        src/pages/websocketserver/SAKWebSocketServerDebugPage.hh \
        src/pages/websocketserver/SAKWebSocketServerDevice.hh \
        src/pages/websocketserver/SAKWebSocketServerDeviceController.hh

    SOURCES += \
        src/pages/websocketclient/SAKWebSocketClientDebugPage.cc \
        src/pages/websocketclient/SAKWebSocketClientDevice.cc \
        src/pages/websocketclient/SAKWebSocketClientDeviceController.cc \
        src/pages/websocketserver/SAKWebSocketServerDebugPage.cc \
        src/pages/websocketserver/SAKWebSocketServerDevice.cc \
        src/pages/websocketserver/SAKWebSocketServerDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/pages/websocketclient \
        $$PWD/src/pages/websocketserver
}else {
    message("The Qt edition has no websocket module, the program will not has websocket debuging functioin.")
}
