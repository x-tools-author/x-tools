include($$PWD/plugin/SAKDebuggerPlugin.pri)

include($$PWD/3d/SAKPlugin3d.pri)
include($$PWD/charts/SAKPluginCharts.pri)
include($$PWD/dataforwarding/SAKPluginDataForwarding.pri)
include($$PWD/regularlysending/SAKPluginRegularlySending.pri)
include($$PWD/automaticallyresponse/SAKPluginAutomaticallyResponse.pri)

HEADERS += \
    $$PWD/SAKDebuggerPlugins.hh

SOURCES += \
    $$PWD/SAKDebuggerPlugins.cc

INCLUDEPATH += \
    $$PWD
