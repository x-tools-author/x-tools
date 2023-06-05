isEqual(QT_MAJOR_VERSION, 5) {
    !lessThan(QT_MINOR_VERSION, 7) {
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_MODBUSUI
        }
    }
} else {
    qtHaveModule(serialbus){
        QT  += serialbus
        DEFINES+=SAK_IMPORT_MODULE_MODBUSUI
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_MODBUSUI) {
    FORMS += \
    $$PWD/SAKModbusUi.ui

    HEADERS += \
    $$PWD/SAKModbusUi.hh

    SOURCES += \
    $$PWD/SAKModbusUi.cc

    INCLUDEPATH += \
        $$PWD
}

