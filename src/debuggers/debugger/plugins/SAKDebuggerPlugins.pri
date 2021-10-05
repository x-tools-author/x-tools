exists($$PWD/plugin/src/SAKDebuggerPlugin.pri) {
    include($$PWD/plugin/src/SAKDebuggerPlugin.pri)
}
include($$PWD/transponders/SAKDebuggerPluginTransponders.pri)
include($$PWD/autoresponse/SAKDebuggerPluginAutoResponse.pri)
include($$PWD/timedsending/SAKDebuggerPluginTimedSending.pri)

HEADERS += \
    $$PWD/SAKDebuggerPlugins.hh

SOURCES += \
    $$PWD/SAKDebuggerPlugins.cc

INCLUDEPATH += \
    $$PWD
