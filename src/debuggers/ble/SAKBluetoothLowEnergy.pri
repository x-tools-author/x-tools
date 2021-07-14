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
        $$PWD/src/debuggers/ble/SAKBluetoothLowEnergyDeviceController.ui

    HEADERS += \
        $$PWD/src/debuggers/ble/SAKBluetoothLowEnergyDebugPage.hh \
        $$PWD/src/debuggers/ble/SAKBluetoothLowEnergyDevice.hh \
        $$PWD/src/debuggers/ble/SAKBluetoothLowEnergyDeviceController.hh

    SOURCES += \
        $$PWD/src/debuggers/ble/SAKBluetoothLowEnergyDebugPage.cc \
        $$PWD/src/debuggers/ble/SAKBluetoothLowEnergyDevice.cc \
        $$PWD/src/debuggers/ble/SAKBluetoothLowEnergyDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/debuggers/ble
} else {
    message("Can not find bluetooth module, The bluetooth debugging will be ignore!")
}
