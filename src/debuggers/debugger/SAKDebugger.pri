include($$PWD/plugins/SAKPlugins.pri)

INCLUDEPATH += \
    $$PWD \
    $$PWD/common \
    $$PWD/controller \
    $$PWD/device \
    $$PWD/input \
    $$PWD/device/analyzer \
    $$PWD/output \
    $$PWD/statistics

FORMS += \
    $$PWD/SAKDebugger.ui \
    #$$PWD/common/SAKDebugPageCommonSslConfigurationWidget.ui \
    $$PWD/device/SAKDebuggerDeviceMask.ui \
    $$PWD/input/SAKDebuggerInputCrcSettings.ui \
    $$PWD/input/SAKDebuggerInputDataPreset.ui \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.ui \
    $$PWD/input/SAKDebuggerInputDataPresetItem.ui \
    $$PWD/output/SAKDebuggerOutputHighlighter.ui \
    $$PWD/output/SAKDebuggerOutputLog.ui \
    $$PWD/output/SAKDebuggerOutputSave2File.ui

HEADERS += \
    $$PWD/SAKDebugger.hh \
    $$PWD/common/SAKDebugPageCommonDatabaseInterface.hh \
    #$$PWD/common/SAKDebugPageCommonSslConfigurationWidget.hh \
    $$PWD/controller/SAKDebuggerController.hh \
    $$PWD/device/SAKDebuggerDevice.hh \
    $$PWD/device/SAKDebuggerDeviceMask.hh \
    $$PWD/input/SAKDebuggerInput.hh \
    $$PWD/input/SAKDebuggerInputCrcSettings.hh \
    $$PWD/device/analyzer/SAKOtherAnalyzerThread.hh \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.hh \
    $$PWD/input/SAKDebuggerInputDataPreset.hh \
    $$PWD/input/SAKDebuggerInputDataPresetItem.hh \
    $$PWD/output/SAKDebuggerOutput.hh \
    $$PWD/output/SAKDebuggerOutputHighlighter.hh \
    $$PWD/output/SAKDebuggerOutputLog.hh \
    $$PWD/output/SAKDebuggerOutputSave2File.hh \
    $$PWD/statistics/SAKDebuggerStatistics.hh

SOURCES += \
    $$PWD/SAKDebugger.cc \
    $$PWD/common/SAKDebugPageCommonDatabaseInterface.cc \
    #$$PWD/common/SAKDebugPageCommonSslConfigurationWidget.cc \
    $$PWD/controller/SAKDebuggerController.cc \
    $$PWD/device/SAKDebuggerDevice.cc \
    $$PWD/device/SAKDebuggerDeviceMask.cc \
    $$PWD/input/SAKDebuggerInput.cc \
    $$PWD/input/SAKDebuggerInputCrcSettings.cc \
    $$PWD/device/analyzer/SAKOtherAnalyzerThread.cc \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.cc \
    $$PWD/input/SAKDebuggerInputDataPreset.cc \
    $$PWD/input/SAKDebuggerInputDataPresetItem.cc \
    $$PWD/output/SAKDebuggerOutput.cc \
    $$PWD/output/SAKDebuggerOutputHighlighter.cc \
    $$PWD/output/SAKDebuggerOutputLog.cc \
    $$PWD/output/SAKDebuggerOutputSave2File.cc \
    $$PWD/statistics/SAKDebuggerStatistics.cc
