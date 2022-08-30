# Serialbus was introduced in Qt 5.8.
isEqual(QT_MAJOR_VERSION, 5){
    !lessThan(QT_MINOR_VERSION, 7){
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_CANBUS_STUDIO
        }
    }
} else {
    qtHaveModule(serialbus){
        QT  += serialbus
        DEFINES+=SAK_IMPORT_MODULE_CANBUS_STUDIO
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_CANBUS_STUDIO) {
    HEADERS += \
    $$PWD/SAKCanBusDebugger.hh

    SOURCES += \
    $$PWD/SAKCanBusDebugger.cc

    FORMS += \
    $$PWD/SAKCanBusDebugger.ui

    INCLUDEPATH += \
        $$PWD
}
