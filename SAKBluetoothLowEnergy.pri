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
        $$PWD/src/pages/bluetooth/ble/SAKBluetoothLowEnergyDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/bluetooth/ble/SAKBluetoothLowEnergyDebugPage.hh \
        $$PWD/src/pages/bluetooth/ble/SAKBluetoothLowEnergyDevice.hh \
        $$PWD/src/pages/bluetooth/ble/SAKBluetoothLowEnergyDeviceController.hh

    SOURCES += \
        $$PWD/src/pages/bluetooth/ble/SAKBluetoothLowEnergyDebugPage.cc \
        $$PWD/src/pages/bluetooth/ble/SAKBluetoothLowEnergyDevice.cc \
        $$PWD/src/pages/bluetooth/ble/SAKBluetoothLowEnergyDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/pages/bluetooth/ble \
        $$PWD/src/pages/bluetooth/ble
} else {
    message("Can not find bluetooth module, The bluetooth debugging will be ignore!")
}
