qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_CHARTS_MODULE
}

contains(DEFINES, SAK_IMPORT_CHARTS_MODULE){
    INCLUDEPATH += \
        $$PWD/ui/widgets/page/visualization \
        $$PWD/ui/widgets/page/visualization/throughput

    FORMS += \
        $$PWD//ui/widgets/page/visualization/SAKDataVisualizationManager.ui

    HEADERS += \
        $$PWD//ui/widgets/page/visualization/SAKDataVisualizationManager.hh \
        $$PWD//ui/widgets/page/visualization/throughput/SAKThroughputWidget.hh

    SOURCES += \
        $$PWD//ui/widgets/page/visualization/SAKDataVisualizationManager.cc \
        $$PWD//ui/widgets/page/visualization/throughput/SAKThroughputWidget.cc
}else {
    warning("The Qt edition has no charts module, the program will not have data visualization functioin.")
}
