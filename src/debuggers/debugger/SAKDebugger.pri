include($$PWD/plugins/SAKPlugins.pri)

INCLUDEPATH += \
    $$PWD \
    $$PWD/common \
    $$PWD/controller \
    $$PWD/device \
    $$PWD/device/mask \
    $$PWD/input \
    $$PWD/input/crcsettings \
    $$PWD/input/datapreset \
    $$PWD/device/analyzer \
    $$PWD/output \
    $$PWD/statistics

FORMS += \
    $$PWD/SAKDebugger.ui \
    $$PWD/common/SAKDebugPageCommonSslConfigurationWidget.ui \
    $$PWD/device/mask/SAKDebugPageDeviceMask.ui \
    $$PWD/input/crcsettings/SAKInputCrcSettingsDialog.ui \
    $$PWD/input/datapreset/SAKInputDataPresetItem.ui \
    $$PWD/input/datapreset/SAKInputDataPresetItemManager.ui \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.ui \
    $$PWD/output/SAKDebuggerOutputHighlighter.ui \
    $$PWD/output/SAKDebuggerOutputLog.ui \
    $$PWD/output/SAKDebuggerOutputSave2File.ui

DISTFILES += \
    $$PWD/README.md

HEADERS += \
    $$PWD/SAKDebugger.hh \
    $$PWD/common/SAKDebugPageCommonDatabaseInterface.hh \
    $$PWD/common/SAKDebugPageCommonSslConfigurationWidget.hh \
    $$PWD/controller/SAKDebugPageController.hh \
    $$PWD/device/SAKDebuggerDevice.hh \
    $$PWD/device/mask/SAKDebugPageDeviceMask.hh \
    $$PWD/input/SAKDebuggerInput.hh \
    $$PWD/input/crcsettings/SAKInputCrcSettingsDialog.hh \
    $$PWD/input/datapreset/SAKInputDataPresetItem.hh \
    $$PWD/input/datapreset/SAKInputDataPresetItemManager.hh \
    $$PWD/device/analyzer/SAKOtherAnalyzerThread.hh \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.hh \
    $$PWD/output/SAKDebuggerOutput.hh \
    $$PWD/output/SAKDebuggerOutputHighlighter.hh \
    $$PWD/output/SAKDebuggerOutputLog.hh \
    $$PWD/output/SAKDebuggerOutputSave2File.hh \
    $$PWD/statistics/SAKDebuggerStatistics.hh

SOURCES += \
    $$PWD/SAKDebugger.cc \
    $$PWD/common/SAKDebugPageCommonDatabaseInterface.cc \
    $$PWD/common/SAKDebugPageCommonSslConfigurationWidget.cc \
    $$PWD/controller/SAKDebugPageController.cc \
    $$PWD/device/SAKDebuggerDevice.cc \
    $$PWD/device/mask/SAKDebugPageDeviceMask.cc \
    $$PWD/input/SAKDebuggerInput.cc \
    $$PWD/input/crcsettings/SAKInputCrcSettingsDialog.cc \
    $$PWD/input/datapreset/SAKInputDataPresetItem.cc \
    $$PWD/input/datapreset/SAKInputDataPresetItemManager.cc \
    $$PWD/device/analyzer/SAKOtherAnalyzerThread.cc \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.cc \
    $$PWD/output/SAKDebuggerOutput.cc \
    $$PWD/output/SAKDebuggerOutputHighlighter.cc \
    $$PWD/output/SAKDebuggerOutputLog.cc \
    $$PWD/output/SAKDebuggerOutputSave2File.cc \
    $$PWD/statistics/SAKDebuggerStatistics.cc
