win32 {
    equals(QT_MAJOR_VERSION, 5) {
        equals(QT_MINOR_VERSION, 15) {
            qtHaveModule(bluetooth) {
                QT  += bluetooth
                DEFINES+=SAK_IMPORT_MODULE_BLUETOOTH
            }
        }
    } else {
        equals(QT_MAJOR_VERSION, 6) {
            qtHaveModule(bluetooth) {
                QT  += bluetooth
                DEFINES+=SAK_IMPORT_MODULE_BLUETOOTH
            }
        }
    }
} else {
    qtHaveModule(bluetooth) {
        QT  += bluetooth
        DEFINES+=SAK_IMPORT_MODULE_BLUETOOTH
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_BLUETOOTH) {
    FORMS += \
        $$PWD/src/pages/bluetooth/client/SAKBluetoothClientDeviceController.ui \
        $$PWD/src/pages/bluetooth/server/SAKBluetoothServerDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/bluetooth/client/SAKBluetoothClientDebugPage.hh \
        $$PWD/src/pages/bluetooth/client/SAKBluetoothClientDevice.hh \
        $$PWD/src/pages/bluetooth/client/SAKBluetoothClientDeviceController.hh \
        $$PWD/src/pages/bluetooth/server/SAKBluetoothServerDebugPage.hh \
        $$PWD/src/pages/bluetooth/server/SAKBluetoothServerDevice.hh \
        $$PWD/src/pages/bluetooth/server/SAKBluetoothServerDeviceController.hh

    SOURCES += \
        $$PWD/src/pages/bluetooth/client/SAKBluetoothClientDebugPage.cc \
        $$PWD/src/pages/bluetooth/client/SAKBluetoothClientDevice.cc \
        $$PWD/src/pages/bluetooth/client/SAKBluetoothClientDeviceController.cc \
        $$PWD/src/pages/bluetooth/server/SAKBluetoothServerDebugPage.cc \
        $$PWD/src/pages/bluetooth/server/SAKBluetoothServerDevice.cc \
        $$PWD/src/pages/bluetooth/server/SAKBluetoothServerDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/pages/bluetooth/client \
        $$PWD/src/pages/bluetooth/server
} else {
    message("Can not find bluetooth module, The bluetooth debugging will be ignore!")
}


