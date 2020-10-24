DEFINES+=SAK_IMPORT_MODULE_SSLSOCKET

contains(DEFINES, SAK_IMPORT_MODULE_SSLSOCKET){
    FORMS += \
        $$PWD/src/pages/sslsocket/client/SAKSslSocketClientDeviceController.ui \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketServerDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/sslsocket/client/SAKSslSocketClientDebugPage.hh \
        $$PWD/src/pages/sslsocket/client/SAKSslSocketClientDevice.hh \
        $$PWD/src/pages/sslsocket/client/SAKSslSocketClientDeviceController.hh \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketServerDebugPage.hh \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketServerDevice.hh \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketServerDeviceController.hh \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketTcpServer.hh

    SOURCES += \
        $$PWD/src/pages/sslsocket/client/SAKSslSocketClientDebugPage.cc \
        $$PWD/src/pages/sslsocket/client/SAKSslSocketClientDevice.cc \
        $$PWD/src/pages/sslsocket/client/SAKSslSocketClientDeviceController.cc \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketServerDebugPage.cc \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketServerDevice.cc \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketServerDeviceController.cc \
        $$PWD/src/pages/sslsocket/server/SAKSslSocketTcpServer.cc
}
