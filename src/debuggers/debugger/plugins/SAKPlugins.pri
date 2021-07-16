include($$PWD/autoresponse/SAKDebuggerPluginAutoResponse.pri)
include($$PWD/charts/SAKDebuggerPluginCharts.pri)
include($$PWD/timingsent/SAKDebuggerRegularlySend.pri)
include($$PWD/transmission/SAKDebuggerPluginTransmission.pri)

HEADERS += \
    $$PWD/SAKDebuggerPlugins.hh

SOURCES += \
    $$PWD/SAKDebuggerPlugins.cc
