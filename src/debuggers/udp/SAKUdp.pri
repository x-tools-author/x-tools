qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_UDP
}

contains(DEFINES, SAK_IMPORT_MODULE_UDP){
    include($$PWD/client/SAKUdpClient.pri)
    include($$PWD/server/SAKUdpServer.pri)
}
