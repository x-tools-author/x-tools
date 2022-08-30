isEqual(QT_MAJOR_VERSION, 5) {
    !lessThan(QT_MINOR_VERSION, 7) {
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_MODBUS_STUDIO
        }
    }
} else {
    qtHaveModule(serialbus){
        QT  += serialbus
        DEFINES+=SAK_IMPORT_MODULE_MODBUS_STUDIO
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_MODBUS_STUDIO) {
    FORMS += \
        $$PWD/SAKModbusStudio.ui

    HEADERS += \
        $$PWD/SAKModbusStudio.hh

    SOURCES += \
        $$PWD/SAKModbusStudio.cc

    INCLUDEPATH += \
        $$PWD
}

