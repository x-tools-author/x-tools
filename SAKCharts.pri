qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_CHARTS_MODULE
}

contains(DEFINES, SAK_IMPORT_CHARTS_MODULE){
    FORMS += \
    $$PWD/src/pages/page/charts/SAKChartsManager.ui

    HEADERS += \
    $$PWD/src/pages/page/charts/SAKChartsManager.hh \
        $$PWD/src/pages/page/charts/throughput/SAKThroughputWidget.hh

    SOURCES += \
    $$PWD/src/pages/page/charts/SAKChartsManager.cc \
        $$PWD/src/pages/page/charts/throughput/SAKThroughputWidget.cc

    INCLUDEPATH += \
        $$PWD/src/pages/page/charts \
        $$PWD/src/pages/page/charts/throughput
}else {
    warning("The Qt edition has no charts module, the program will not have data visualization functioin.")
}
