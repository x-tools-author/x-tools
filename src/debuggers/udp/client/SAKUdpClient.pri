qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_UDP_CLIENT
}

contains(DEFINES, SAK_IMPORT_MODULE_UDP_CLIENT){
FORMS += \
    $$PWD/SAKUdpClientAdvanced.ui \
    $$PWD/SAKUdpClientController.ui

HEADERS += \
    $$PWD/SAKUdpClientAdvanced.hh \
    $$PWD/SAKUdpClientController.hh \
    $$PWD/SAKUdpClientDebugger.hh \
    $$PWD/SAKUdpClientDevice.hh

SOURCES += \
    $$PWD/SAKUdpClientAdvanced.cc \
    $$PWD/SAKUdpClientController.cc \
    $$PWD/SAKUdpClientDebugger.cc \
    $$PWD/SAKUdpClientDevice.cc \

INCLUDEPATH += \
    $$PWD
}

