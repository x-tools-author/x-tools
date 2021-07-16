include($$PWD/charts/SAKCharts.pri)
include($$PWD/timingsent/SAKRegularlySend.pri)
include($$PWD/transmission/SAKTransmission.pri)
include($$PWD/autoresponse/SAKAutoResponse.pri)

HEADERS += \
    $$PWD/SAKDebuggerPlugins.hh

SOURCES += \
    $$PWD/SAKDebuggerPlugins.cc

INCLUDEPATH += \
    $$PWD
