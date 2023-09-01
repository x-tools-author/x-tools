isEqual(QT_MAJOR_VERSION, 5) {
    !lessThan(QT_MINOR_VERSION, 7) {
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_MODBUS
        }
    }
} else {
    qtHaveModule(serialbus){
        QT  += serialbus
        DEFINES+=SAK_IMPORT_MODULE_MODBUS
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_MODBUS) {
    include($$PWD/modbus/sakmodbus.pri)
    include($$PWD/modbusui/sakmodbusui.pri)
}
