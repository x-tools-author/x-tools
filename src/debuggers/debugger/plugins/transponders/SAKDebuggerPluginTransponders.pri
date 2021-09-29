FORMS += \
    $$PWD/SAKDebuggerPluginTransponders.ui
HEADERS += \
    $$PWD/SAKDebuggerPluginTransponders.hh \
    $$PWD/SAKTransponder.hh
SOURCES += \
    $$PWD/SAKDebuggerPluginTransponders.cc \
    $$PWD/SAKTransponder.cc
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
