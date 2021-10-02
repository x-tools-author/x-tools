qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_TCP
}

contains(DEFINES, SAK_IMPORT_MODULE_TCP){
    include($$PWD/client/SAKTcpClient.pri)
    include($$PWD/server/SAKTcpServer.pri)
}
