qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_UDP
}

contains(DEFINES, SAK_IMPORT_MODULE_UDP){
    INCLUDEPATH += \
        src/pages/udp/client \
        src/pages/udp/server

    FORMS += \
        src/pages/udp/client/SAKUdpClientAdvanceSettingWidget.ui \
        src/pages/udp/client/SAKUdpClientDeviceController.ui \
        src/pages/udp/client/SAKUdpClientMulticastEditingDialog.ui \
        src/pages/udp/server/SAKUdpServerDeviceController.ui

    HEADERS += \
        src/pages/udp/client/SAKUdpClientAdvanceSettingWidget.hh \
        src/pages/udp/client/SAKUdpClientDebugPage.hh \
        src/pages/udp/client/SAKUdpClientDevice.hh \
        src/pages/udp/client/SAKUdpClientDeviceController.hh \
        src/pages/udp/client/SAKUdpClientMulticastEditingDialog.hh \
        src/pages/udp/server/SAKUdpServerDebugPage.hh \
        src/pages/udp/server/SAKUdpServerDevice.hh \
        src/pages/udp/server/SAKUdpServerDeviceController.hh

    SOURCES += \
        src/pages/udp/client/SAKUdpClientAdvanceSettingWidget.cc \
        src/pages/udp/client/SAKUdpClientDebugPage.cc \
        src/pages/udp/client/SAKUdpClientDevice.cc \
        src/pages/udp/client/SAKUdpClientDeviceController.cc \
        src/pages/udp/client/SAKUdpClientMulticastEditingDialog.cc \
        src/pages/udp/server/SAKUdpServerDebugPage.cc \
        src/pages/udp/server/SAKUdpServerDevice.cc \
        src/pages/udp/server/SAKUdpServerDeviceController.cc
}
