qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_CHARTS_MODULE
}

contains(DEFINES, SAK_IMPORT_CHARTS_MODULE){
    FORMS += \
        $$PWD/src/base/page/visualization/SAKDataVisualizationManager.ui

    HEADERS += \
        $$PWD/src/base/page/visualization/SAKDataVisualizationManager.hh \
        $$PWD/src/base/page/visualization/throughput/SAKThroughputWidget.hh

    SOURCES += \
        $$PWD/src/base/page/visualization/SAKDataVisualizationManager.cc \
        $$PWD/src/base/page/visualization/throughput/SAKThroughputWidget.cc

    INCLUDEPATH += \
        $$PWD/src/base/page/visualization \
        $$PWD/src/base/page/visualization/throughput
}else {
    warning("The Qt edition has no charts module, the program will not have data visualization functioin.")
}
