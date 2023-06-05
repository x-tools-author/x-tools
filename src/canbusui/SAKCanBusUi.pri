# Serialbus was introduced in Qt 5.8.
isEqual(QT_MAJOR_VERSION, 5){
    !lessThan(QT_MINOR_VERSION, 7){
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_CANBUSUI
        }
    }
} else {
    qtHaveModule(serialbus){
        QT  += serialbus
        DEFINES+=SAK_IMPORT_MODULE_CANBUSUI
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_CANBUSUI) {
    HEADERS += \
    $$PWD/SAKCanBusUi.hh

    SOURCES += \
    $$PWD/SAKCanBusUi.cc

    FORMS += \
    $$PWD/SAKCanBusUi.ui

    INCLUDEPATH += \
        $$PWD
}
