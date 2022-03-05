qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_UDP_SERVER
}

contains(DEFINES, SAK_IMPORT_MODULE_UDP_SERVER){
FORMS += \
    $$PWD/SAKUdpServerMulticast.ui \
    $$PWD/SAKUdpServerController.ui

HEADERS += \
    $$PWD/SAKUdpServerController.hh \
    $$PWD/SAKUdpServerDebugger.hh \
    $$PWD/SAKUdpServerDevice.hh \
    $$PWD/SAKUdpServerMulticast.hh

SOURCES += \
    $$PWD/SAKUdpServerController.cc \
    $$PWD/SAKUdpServerDebugger.cc \
    $$PWD/SAKUdpServerDevice.cc \
    $$PWD/SAKUdpServerMulticast.cc

INCLUDEPATH += \
    $$PWD
}
