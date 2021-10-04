exists($$PWD/plugin/src/SAKDebuggerPlugin.pri) {
    include($$PWD/plugin/src/SAKDebuggerPlugin.pri)
}
include($$PWD/transponders/SAKDebuggerPluginTransponders.pri)
include($$PWD/autoresponse/SAKDebuggerPluginAutoResponse.pri)
include($$PWD/timedsending/SAKDebuggerPluginTimedSending.pri)

HEADERS += \
    $$PWD/SAKDebuggerPlugins.hh \
    $$PWD/manager/SAKDebuggerPluginsManager.hh

SOURCES += \
    $$PWD/SAKDebuggerPlugins.cc \
    $$PWD/manager/SAKDebuggerPluginsManager.cc

INCLUDEPATH += \
    $$PWD \
    $$PWD/manager \

FORMS += \
    $$PWD/manager/SAKDebuggerPluginsManager.ui
