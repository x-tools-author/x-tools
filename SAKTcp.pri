qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_TCP
}

contains(DEFINES, SAK_IMPORT_MODULE_TCP){
    INCLUDEPATH += \
        src/pages/tcp/client \
        src/pages/tcp/server

    FORMS += \
        src/pages/tcp/client/SAKTcpClientDeviceController.ui \
        src/pages/tcp/server/SAKTcpServerDeviceController.ui

    HEADERS += \
        src/pages/tcp/client/SAKTcpClientDebugPage.hh \
        src/pages/tcp/client/SAKTcpClientDevice.hh \
        src/pages/tcp/client/SAKTcpClientDeviceController.hh \
        src/pages/tcp/server/SAKTcpServerDebugPage.hh \
        src/pages/tcp/server/SAKTcpServerDevice.hh \
        src/pages/tcp/server/SAKTcpServerDeviceController.hh

    SOURCES += \
        src/pages/tcp/client/SAKTcpClientDebugPage.cc \
        src/pages/tcp/client/SAKTcpClientDevice.cc \
        src/pages/tcp/client/SAKTcpClientDeviceController.cc \
        src/pages/tcp/server/SAKTcpServerDebugPage.cc \
        src/pages/tcp/server/SAKTcpServerDevice.cc \
        src/pages/tcp/server/SAKTcpServerDeviceController.cc
}
