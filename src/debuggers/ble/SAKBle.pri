win32 {
    equals(QT_MAJOR_VERSION, 5) {
        equals(QT_MINOR_VERSION, 15) {
            qtHaveModule(bluetooth) {
                DEFINES+=SAK_IMPORT_MODULE_BLE
            }
        }
    } else {
        qtHaveModule(bluetooth) {
            DEFINES+=SAK_IMPORT_MODULE_BLE
        }
    }
} else {
    qtHaveModule(bluetooth) {
        DEFINES+=SAK_IMPORT_MODULE_BLE
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_BLE) {
    QT  += bluetooth
    include($$PWD/central/SAKBleCentral.pri)
}
