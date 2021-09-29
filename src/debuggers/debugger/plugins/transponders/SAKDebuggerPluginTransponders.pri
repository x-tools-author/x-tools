FORMS += \
    $$PWD/SAKDebuggerPluginTransponders.ui \
    $$PWD/SAKTcpTransponder.ui \
    $$PWD/SAKUdpTransponder.ui \
    $$PWD/SAKWebSocketTransponder.ui
HEADERS += \
    $$PWD/SAKDebuggerPluginTransponder.hh \
    $$PWD/SAKDebuggerPluginTransponders.hh \
    $$PWD/SAKTcpTransponder.hh \
    $$PWD/SAKTcpTransponders.hh \
    $$PWD/SAKWebSocketTransponder.hh \
    $$PWD/SAKWebSocketTransponders.hh
SOURCES += \
    $$PWD/SAKDebuggerPluginTransponder.cc \
    $$PWD/SAKDebuggerPluginTransponders.cc \
    $$PWD/SAKTcpTransponder.cc \
    $$PWD/SAKTcpTransponders.cc \
    $$PWD/SAKWebSocketTransponder.cc \
    $$PWD/SAKWebSocketTransponders.cc
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
HEADERS += \
    $$PWD/SAKUdpTransponder.hh \
    $$PWD/SAKUdpTransponders.hh
SOURCES += \
    $$PWD/SAKUdpTransponder.cc \
    $$PWD/SAKUdpTransponders.cc
}
