qtHaveModule(websockets){
    QT  += websockets
    DEFINES+=SAK_IMPORT_MODULE_WEBSOCKET
}

contains(DEFINES, SAK_IMPORT_MODULE_WEBSOCKET){
    include($$PWD/client/SAKWebSocketClient.pri)
    include($$PWD/server/SAKWebSocketServer.pri)
} else {
    message("The Qt edition has no websocket module, the program will not has websocket debugging function.")
}
