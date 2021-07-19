include($$PWD/charts/SAKPluginCharts.pri)
include($$PWD/automaticallyresponse/SAKPluginAutomaticallyResponse.pri)
include($$PWD/dataforwarding/SAKPluginDataForwarding.pri)
include($$PWD/regularlysending/SAKPluginRegularlySending.pri)

HEADERS += \
    $$PWD/SAKDebuggerPlugins.hh

SOURCES += \
    $$PWD/SAKDebuggerPlugins.cc

INCLUDEPATH += \
    $$PWD
