qtHaveModule(bluetooth){
    QT  += bluetooth
    DEFINES+=SAK_IMPORT_BLUETOOTH_MODULE
}

contains(DEFINES, SAK_IMPORT_BLUETOOTH_MODULE){
INCLUDEPATH += \
    $$PWD/src/pages/bluetooth/client \
    $$PWD/src/pages/bluetooth/server

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
}else {
    message("The Qt has no bluetooth module, the program will not has bluetooth debuging function.")
}
