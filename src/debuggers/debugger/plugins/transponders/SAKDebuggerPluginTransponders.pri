FORMS += \
    $$PWD/SAKDebuggerPluginTransponders.ui \
    $$PWD/SAKUdpTransponder.ui
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
HEADERS += \
    $$PWD/SAKUdpTransponder.hh \
    $$PWD/SAKUdpTransponders.hh
SOURCES += \
    $$PWD/SAKUdpTransponder.cc \
    $$PWD/SAKUdpTransponders.cc
}
