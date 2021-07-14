win32 {
    equals(QT_MAJOR_VERSION, 5) {
        equals(QT_MINOR_VERSION, 15) {
            qtHaveModule(bluetooth) {
                QT  += bluetooth
                DEFINES+=SAK_IMPORT_MODULE_BLUETOOTHLOWENERGY
            }
        }
    } else {
        equals(QT_MAJOR_VERSION, 6) {
            qtHaveModule(bluetooth) {
                QT  += bluetooth
                DEFINES+=SAK_IMPORT_MODULE_BLUETOOTHLOWENERGY
            }
        }
    }
} else {
    qtHaveModule(bluetooth) {
        QT  += bluetooth
        DEFINES+=SAK_IMPORT_MODULE_BLUETOOTHLOWENERGY
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_BLUETOOTHLOWENERGY) {
    FORMS += \
        $$PWD/src/debuggers/bluetoothlowenergy/SAKBluetoothLowEnergyDeviceController.ui

    HEADERS += \
        $$PWD/src/debuggers/bluetoothlowenergy/SAKBluetoothLowEnergyDebugPage.hh \
        $$PWD/src/debuggers/bluetoothlowenergy/SAKBluetoothLowEnergyDevice.hh \
        $$PWD/src/debuggers/bluetoothlowenergy/SAKBluetoothLowEnergyDeviceController.hh

    SOURCES += \
        $$PWD/src/debuggers/bluetoothlowenergy/SAKBluetoothLowEnergyDebugPage.cc \
        $$PWD/src/debuggers/bluetoothlowenergy/SAKBluetoothLowEnergyDevice.cc \
        $$PWD/src/debuggers/bluetoothlowenergy/SAKBluetoothLowEnergyDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/debuggers/bluetoothlowenergy
} else {
    message("Can not find bluetooth module, The bluetooth debugging will be ignore!")
}
