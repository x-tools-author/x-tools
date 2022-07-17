# Serialbus was introduced in Qt 5.8.
exists(private/modbus/modbus/SAKModbus.pri) {
    isEqual(QT_MAJOR_VERSION, 5){
        !lessThan(QT_MINOR_VERSION, 7){
            qtHaveModule(serialbus){
                QT  += serialbus
                DEFINES+=SAK_IMPORT_MODULE_STUDIO
                include(private/modbus/modbus/SAKModbus.pri)
            }
        }
    } else {
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_STUDIO
            include(private/modbus/modbus/SAKModbus.pri)
        }
    }
}
