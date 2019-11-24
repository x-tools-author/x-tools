qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_CHARTS_MODULE
}

contains(DEFINES, SAK_IMPORT_CHARTS_MODULE){
    INCLUDEPATH += \
        $$PWD/src/page/visualization \
        $$PWD/src/page/visualization/throughput

    FORMS += \
        $$PWD/src/page/visualization/SAKDataVisualizationManager.ui

    HEADERS += \
        $$PWD/src/page/visualization/SAKDataVisualizationManager.hh \
        $$PWD/src/page/visualization/throughput/SAKThroughputWidget.hh

    SOURCES += \
        $$PWD/src/page/visualization/SAKDataVisualizationManager.cc \
        $$PWD/src/page/visualization/throughput/SAKThroughputWidget.cc
}else {
    warning("The Qt edition has no charts module, the program will not have data visualization functioin.")
}
