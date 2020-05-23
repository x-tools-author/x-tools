qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_CHARTS_MODULE
}

contains(DEFINES, SAK_IMPORT_CHARTS_MODULE){
    FORMS += \
        $$PWD/src/pages/page/visualization/SAKDataVisualizationManager.ui

    HEADERS += \
        $$PWD/src/pages/page/visualization/SAKDataVisualizationManager.hh \
        $$PWD/src/pages/page/visualization/throughput/SAKThroughputWidget.hh

    SOURCES += \
        $$PWD/src/pages/page/visualization/SAKDataVisualizationManager.cc \
        $$PWD/src/pages/page/visualization/throughput/SAKThroughputWidget.cc

    INCLUDEPATH += \
        $$PWD/src/pages/page/visualization \
        $$PWD/src/pages/page/visualization/throughput
}else {
    warning("The Qt edition has no charts module, the program will not have data visualization functioin.")
}
