qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_MODULE_CHARTS
}

contains(DEFINES, SAK_IMPORT_MODULE_CHARTS){
INCLUDEPATH += \
    $$PWD/src/pages/page/charts \
    $$PWD/src/pages/page/charts/throughput \
    $$PWD/src/pages/page/charts/xyserial

FORMS += \
    $$PWD/src/pages/page/charts/SAKDebugPageChartsController.ui \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialChartViewSettingsDialog.ui \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialWidget.ui \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialEditDialog.ui

HEADERS += \
    $$PWD/src/pages/page/charts/SAKDebugPageChartsController.hh \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialChartView.hh \
    $$PWD/src/pages/page/charts/throughput/SAKChartsThroughputWidget.hh \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialChartViewSettingsDialog.hh \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialEditDialog.hh \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialWidget.hh

SOURCES += \
    $$PWD/src/pages/page/charts/SAKDebugPageChartsController.cc \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialChartView.cc \
    $$PWD/src/pages/page/charts/throughput/SAKChartsThroughputWidget.cc \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialChartViewSettingsDialog.cc \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialEditDialog.cc \
    $$PWD/src/pages/page/charts/xyserial/SAKChartsXYSerialWidget.cc
}else {
    message("The Qt edition has no charts module, the program will not have chart debugging function.")
}
