QT  += printsupport
DEFINES += SAK_IMPORT_MODULE_CHARTS

HEADERS += \
    $$PWD/qcustomplot/qcustomplot.h

SOURCES += \
    $$PWD/qcustomplot/qcustomplot.cpp

INCLUDEPATH += \
    $$PWD \
    $$PWD/throughput \
    $$PWD/xyserial

FORMS += \
    $$PWD/SAKPluginCharts.ui \
    $$PWD/xyserial/SAKChartsXYSerial.ui

HEADERS += \
    $$PWD/SAKPluginCharts.hh \
    $$PWD/xyserial/SAKChartsXYSerial.hh

SOURCES += \
    $$PWD/SAKPluginCharts.cc \
    $$PWD/xyserial/SAKChartsXYSerial.cc

