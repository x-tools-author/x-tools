qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_CHARTS_MODULE
}

contains(DEFINES, SAK_IMPORT_CHARTS_MODULE){
    INCLUDEPATH += \
        $$PWD/src/pages/page/charts \
        $$PWD/src/pages/page/charts/throughput \
        $$PWD/src/pages/page/charts/xyserial

    FORMS += \
        $$PWD/src/pages/page/charts/SAKChartsManager.ui \
        $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialWidget.ui \
        $$PWD/src/pages/page/charts/xyserial/SAKXYSerialEditDialog.ui

    HEADERS += \
        $$PWD/src/pages/page/charts/SAKChartsManager.hh \
        $$PWD/src/pages/page/charts/throughput/SAKChartsThroughputWidget.hh \
        $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialWidget.hh \
        $$PWD/src/pages/page/charts/xyserial/SAKXYSerialEditDialog.hh

    SOURCES += \
        $$PWD/src/pages/page/charts/SAKChartsManager.cc \
        $$PWD/src/pages/page/charts/throughput/SAKChartsThroughputWidget.cc \
        $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialWidget.cc \
        $$PWD/src/pages/page/charts/xyserial/SAKXYSerialEditDialog.cc
}else {
    message("The Qt edition has no charts module, the program will not have chart viewer.")
}
