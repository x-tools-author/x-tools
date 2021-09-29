FORMS += \
    $$PWD/SAKDebuggerPluginTransponders.ui
HEADERS += \
    $$PWD/SAKDebuggerPluginTransponders.hh \
    $$PWD/SAKTransponderItem.hh
SOURCES += \
    $$PWD/SAKDebuggerPluginTransponders.cc \
    $$PWD/SAKTransponderItem.cc
INCLUDEPATH += \
    $$PWD

contains(DEFINES, SAK_IMPORT_MODULE_SERIALPORT){
FORMS += \
    $$PWD/SAKTransponderSerialPortItem.ui
HEADERS += \
    $$PWD/SAKTransponderSerialPort.hh \
    $$PWD/SAKTransponderSerialPortItem.hh
SOURCES += \
    $$PWD/SAKTransponderSerialPort.cc \
    $$PWD/SAKTransponderSerialPortItem.cc
}
