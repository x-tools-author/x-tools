qtHaveModule(charts){
    QT  += charts
    DEFINES+=SAK_IMPORT_MODULE_CHARTS
}

contains(DEFINES, SAK_IMPORT_MODULE_CHARTS){
INCLUDEPATH += \
    $$PWD/src/debuggers/debugger/charts \
    $$PWD/src/debuggers/debugger/charts/throughput \
    $$PWD/src/debuggers/debugger/charts/xyserial

FORMS += \
    $$PWD/src/debuggers/debugger/charts/SAKDebugPageChartsController.ui \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialChartViewSettingsDialog.ui \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialWidget.ui \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialEditDialog.ui

HEADERS += \
    $$PWD/src/debuggers/debugger/charts/SAKDebugPageChartsController.hh \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialChartView.hh \
    $$PWD/src/debuggers/debugger/charts/throughput/SAKChartsThroughputWidget.hh \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialChartViewSettingsDialog.hh \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialEditDialog.hh \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialWidget.hh

SOURCES += \
    $$PWD/src/debuggers/debugger/charts/SAKDebugPageChartsController.cc \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialChartView.cc \
    $$PWD/src/debuggers/debugger/charts/throughput/SAKChartsThroughputWidget.cc \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialChartViewSettingsDialog.cc \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialEditDialog.cc \
    $$PWD/src/debuggers/debugger/charts/xyserial/SAKChartsXYSerialWidget.cc
}else {
    message("The Qt edition has no charts module, the program will not have chart debugging function.")
}
