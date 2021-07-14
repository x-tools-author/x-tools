qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_UDP
}

contains(DEFINES, SAK_IMPORT_MODULE_UDP){
    INCLUDEPATH += \
        src/debuggers/udp/client \
        src/debuggers/udp/server

    FORMS += \
        src/debuggers/udp/client/SAKUdpClientAdvanceSettingWidget.ui \
        src/debuggers/udp/client/SAKUdpClientDeviceController.ui \
        src/debuggers/udp/client/SAKUdpClientMulticastEditingDialog.ui \
        src/debuggers/udp/server/SAKUdpServerDeviceController.ui

    HEADERS += \
        src/debuggers/udp/client/SAKUdpClientAdvanceSettingWidget.hh \
        src/debuggers/udp/client/SAKUdpClientDebugPage.hh \
        src/debuggers/udp/client/SAKUdpClientDevice.hh \
        src/debuggers/udp/client/SAKUdpClientDeviceController.hh \
        src/debuggers/udp/client/SAKUdpClientMulticastEditingDialog.hh \
        src/debuggers/udp/server/SAKUdpServerDebugPage.hh \
        src/debuggers/udp/server/SAKUdpServerDevice.hh \
        src/debuggers/udp/server/SAKUdpServerDeviceController.hh

    SOURCES += \
        src/debuggers/udp/client/SAKUdpClientAdvanceSettingWidget.cc \
        src/debuggers/udp/client/SAKUdpClientDebugPage.cc \
        src/debuggers/udp/client/SAKUdpClientDevice.cc \
        src/debuggers/udp/client/SAKUdpClientDeviceController.cc \
        src/debuggers/udp/client/SAKUdpClientMulticastEditingDialog.cc \
        src/debuggers/udp/server/SAKUdpServerDebugPage.cc \
        src/debuggers/udp/server/SAKUdpServerDevice.cc \
        src/debuggers/udp/server/SAKUdpServerDeviceController.cc
}
