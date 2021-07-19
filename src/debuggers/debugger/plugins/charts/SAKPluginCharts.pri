qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_MODULE_CHARTS
}

contains(DEFINES, SAK_IMPORT_MODULE_CHARTS){
INCLUDEPATH += \
    $$PWD \
    $$PWD/throughput \
    $$PWD/xyserial

FORMS += \
    $$PWD/SAKDebugPageChartsController.ui \
    $$PWD/xyserial/SAKChartsXYSerialChartViewSettingsDialog.ui \
    $$PWD/xyserial/SAKChartsXYSerialEditDialog.ui \
    $$PWD/xyserial/SAKChartsXYSerialWidget.ui

HEADERS += \
    $$PWD/SAKDebugPageChartsController.hh \
    $$PWD/throughput/SAKChartsThroughputWidget.hh \
    $$PWD/xyserial/SAKChartsXYSerialChartView.hh \
    $$PWD/xyserial/SAKChartsXYSerialChartViewSettingsDialog.hh \
    $$PWD/xyserial/SAKChartsXYSerialEditDialog.hh \
    $$PWD/xyserial/SAKChartsXYSerialWidget.hh

SOURCES += \
    $$PWD/SAKDebugPageChartsController.cc \
    $$PWD/throughput/SAKChartsThroughputWidget.cc \
    $$PWD/xyserial/SAKChartsXYSerialChartView.cc \
    $$PWD/xyserial/SAKChartsXYSerialChartViewSettingsDialog.cc \
    $$PWD/xyserial/SAKChartsXYSerialEditDialog.cc \
    $$PWD/xyserial/SAKChartsXYSerialWidget.cc
}else {
    message("The Qt edition has no charts module, the program will not have chart debugging function.")
}
