# Serialbus was introduced in Qt 5.8.
isEqual(QT_MAJOR_VERSION, 5){
    !lessThan(QT_MINOR_VERSION, 7){
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_CANBUS
        }
    }
} else {
    qtHaveModule(serialbus){
        QT  += serialbus
        DEFINES+=SAK_IMPORT_MODULE_CANBUS
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_CANBUS) {
    HEADERS += \
    $$PWD/SAKCanBus.hh

    SOURCES += \
    $$PWD/SAKCanBus.cc

    INCLUDEPATH += \
        $$PWD
}
