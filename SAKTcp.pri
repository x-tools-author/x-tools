qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_TCP
}

contains(DEFINES, SAK_IMPORT_MODULE_TCP){
    INCLUDEPATH += \
        src/debuggers/tcp/client \
        src/debuggers/tcp/server

    FORMS += \
        src/debuggers/tcp/client/SAKTcpClientDeviceController.ui \
        src/debuggers/tcp/server/SAKTcpServerDeviceController.ui

    HEADERS += \
        src/debuggers/tcp/client/SAKTcpClientDebugPage.hh \
        src/debuggers/tcp/client/SAKTcpClientDevice.hh \
        src/debuggers/tcp/client/SAKTcpClientDeviceController.hh \
        src/debuggers/tcp/server/SAKTcpServerDebugPage.hh \
        src/debuggers/tcp/server/SAKTcpServerDevice.hh \
        src/debuggers/tcp/server/SAKTcpServerDeviceController.hh

    SOURCES += \
        src/debuggers/tcp/client/SAKTcpClientDebugPage.cc \
        src/debuggers/tcp/client/SAKTcpClientDevice.cc \
        src/debuggers/tcp/client/SAKTcpClientDeviceController.cc \
        src/debuggers/tcp/server/SAKTcpServerDebugPage.cc \
        src/debuggers/tcp/server/SAKTcpServerDevice.cc \
        src/debuggers/tcp/server/SAKTcpServerDeviceController.cc
}
