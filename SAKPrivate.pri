# Serialbus was introduced in Qt 5.8.
exists(private/QtModbusStudio/modbusstudio/SAKModbusStudio.pri) {
    isEqual(QT_MAJOR_VERSION, 5){
        !lessThan(QT_MINOR_VERSION, 7){
            qtHaveModule(serialbus){
                QT  += serialbus
                DEFINES+=SAK_IMPORT_MODULE_MODBUS_STUDIO
                include(private/QtModbusStudio/modbusstudio/SAKModbusStudio.pri)
            }
        }
    } else {
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_MODBUS_STUDIO
            include(private/QtModbusStudio/modbusstudio/SAKModbusStudio.pri)
        }
    }
}

exists(private/QtCanStudio/canstudio/SAKCanStudio.pri) {
    isEqual(QT_MAJOR_VERSION, 5){
        !lessThan(QT_MINOR_VERSION, 7){
            qtHaveModule(serialbus){
                QT  += serialbus
                DEFINES+=SAK_IMPORT_MODULE_CANBUS_STUDIO
                include(private/QtCanStudio/canstudio/SAKCanStudio.pri)
            }
        }
    } else {
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_CANBUS_STUDIO
            include(private/QtCanStudio/canstudio/SAKCanStudio.pri)
        }
    }
}
