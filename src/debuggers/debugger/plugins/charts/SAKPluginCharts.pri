QT  += printsupport

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
    $$PWD/SAKPluginCharts.ui \
    $$PWD/xyserial/SAKChartsXYSerial.ui \
    $$PWD/xyserial/SAKChartsXYSerialChartViewSettingsDialog.ui \
    $$PWD/xyserial/SAKChartsXYSerialEditDialog.ui

HEADERS += \
    $$PWD/SAKPluginCharts.hh \
    $$PWD/throughput/SAKChartsThroughputWidget.hh \
    $$PWD/xyserial/SAKChartsXYSerial.hh \
    $$PWD/xyserial/SAKChartsXYSerialChartView.hh \
    $$PWD/xyserial/SAKChartsXYSerialChartViewSettingsDialog.hh \
    $$PWD/xyserial/SAKChartsXYSerialEditDialog.hh

SOURCES += \
    $$PWD/SAKPluginCharts.cc \
    $$PWD/throughput/SAKChartsThroughputWidget.cc \
    $$PWD/xyserial/SAKChartsXYSerial.cc \
    $$PWD/xyserial/SAKChartsXYSerialChartView.cc \
    $$PWD/xyserial/SAKChartsXYSerialChartViewSettingsDialog.cc \
    $$PWD/xyserial/SAKChartsXYSerialEditDialog.cc
}else {
    message("The Qt edition has no charts module, the program will not have chart debugging function.")
}

HEADERS += \
    $$PWD/qcustomplot/qcustomplot.h

SOURCES += \
    $$PWD/qcustomplot/qcustomplot.cpp
