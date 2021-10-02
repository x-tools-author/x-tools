win32 {
    equals(QT_MAJOR_VERSION, 5) {
        equals(QT_MINOR_VERSION, 15) {
            qtHaveModule(bluetooth) {
                QT  += bluetooth
                DEFINES+=SAK_IMPORT_MODULE_BLE
            }
        }
    } else {
        equals(QT_MAJOR_VERSION, 6) {
            qtHaveModule(bluetooth) {
                QT  += bluetooth
                DEFINES+=SAK_IMPORT_MODULE_BLE
            }
        }
    }
} else {
    qtHaveModule(bluetooth) {
        QT  += bluetooth
        DEFINES+=SAK_IMPORT_MODULE_BLE
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_BLE) {
    FORMS += \
        $$PWD/SAKBluetoothLowEnergyDeviceController.ui

    HEADERS += \
        $$PWD/SAKBluetoothLowEnergyDebugPage.hh \
        $$PWD/SAKBluetoothLowEnergyDevice.hh \
        $$PWD/SAKBluetoothLowEnergyDeviceController.hh

    SOURCES += \
        $$PWD/SAKBluetoothLowEnergyDebugPage.cc \
        $$PWD/SAKBluetoothLowEnergyDevice.cc \
        $$PWD/SAKBluetoothLowEnergyDeviceController.cc

    INCLUDEPATH += \
        $$PWD
} else {
    message("Can not find bluetooth module, The bluetooth debugging will be ignore!")
}


