FORMS += \
    $$PWD/SAKDebuggerPluginTransponders.ui
HEADERS += \
    $$PWD/SAKDebuggerPluginTransponder.hh \
    $$PWD/SAKDebuggerPluginTransponders.hh
SOURCES += \
    $$PWD/SAKDebuggerPluginTransponder.cc \
    $$PWD/SAKDebuggerPluginTransponders.cc
INCLUDEPATH += \
    $$PWD

contains(DEFINES, SAK_IMPORT_MODULE_SERIALPORT){
    FORMS += \
        $$PWD/SAKSerialPortTransponder.ui
    HEADERS += \
        $$PWD/SAKSerialPortTransponder.hh \
        $$PWD/SAKSerialPortTransponders.hh
    SOURCES += \
        $$PWD/SAKSerialPortTransponder.cc \
        $$PWD/SAKSerialPortTransponders.cc
}

contains(DEFINES, SAK_IMPORT_MODULE_UDP_CLIENT){
FORMS += \
    $$PWD/SAKUdpTransponder.ui
HEADERS += \
    $$PWD/SAKUdpTransponder.hh \
    $$PWD/SAKUdpTransponders.hh
SOURCES += \
    $$PWD/SAKUdpTransponder.cc \
    $$PWD/SAKUdpTransponders.cc
}

contains(DEFINES, SAK_IMPORT_MODULE_TCP_CLIENT){
FORMS += \
    $$PWD/SAKTcpTransponder.ui
HEADERS += \
    $$PWD/SAKTcpTransponder.hh \
    $$PWD/SAKTcpTransponders.hh
SOURCES += \
    $$PWD/SAKTcpTransponder.cc \
    $$PWD/SAKTcpTransponders.cc
}

contains(DEFINES, SAK_IMPORT_MODULE_WEBSOCKET_CLIENT){
FORMS += \
    $$PWD/SAKWebSocketTransponder.ui
HEADERS += \
    $$PWD/SAKWebSocketTransponder.hh \
    $$PWD/SAKWebSocketTransponders.hh
SOURCES += \
    $$PWD/SAKWebSocketTransponder.cc \
    $$PWD/SAKWebSocketTransponders.cc
}
