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
        $$PWD/src/pages/bluetoothlowenergy/SAKBluetoothLowEnergyDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/bluetoothlowenergy/SAKBluetoothLowEnergyDebugPage.hh \
        $$PWD/src/pages/bluetoothlowenergy/SAKBluetoothLowEnergyDevice.hh \
        $$PWD/src/pages/bluetoothlowenergy/SAKBluetoothLowEnergyDeviceController.hh

    SOURCES += \
        $$PWD/src/pages/bluetoothlowenergy/SAKBluetoothLowEnergyDebugPage.cc \
        $$PWD/src/pages/bluetoothlowenergy/SAKBluetoothLowEnergyDevice.cc \
        $$PWD/src/pages/bluetoothlowenergy/SAKBluetoothLowEnergyDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/pages/bluetoothlowenergy
} else {
    message("Can not find bluetooth module, The bluetooth debugging will be ignore!")
}
