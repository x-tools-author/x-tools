include($$PWD/plugins/SAKDebuggerPlugins.pri)

INCLUDEPATH += \
    $$PWD \
    $$PWD/common \
    $$PWD/controller \
    $$PWD/device \
    $$PWD/device/mask \
    $$PWD/input \
    $$PWD/input/crcsettings \
    $$PWD/input/datafactory \
    $$PWD/input/datapreset \
    $$PWD/plugins/autoresponse \
    $$PWD/plugins/highlighter \
    $$PWD/plugins/generator \
    $$PWD/device/analyzer \
    $$PWD/plugins/timingsent \
    $$PWD/plugins/transmission \
    $$PWD/output \
    $$PWD/statistics \
    $$PWD/plugins

FORMS += \
    $$PWD/SAKDebugger.ui \
    $$PWD/common/SAKDebugPageCommonSslConfigurationWidget.ui \
    $$PWD/device/mask/SAKDebugPageDeviceMask.ui \
    $$PWD/input/crcsettings/SAKInputCrcSettingsDialog.ui \
    $$PWD/input/datapreset/SAKInputDataPresetItem.ui \
    $$PWD/input/datapreset/SAKInputDataPresetItemManager.ui \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.ui \
    $$PWD/plugins/autoresponse/SAKOtherAutoResponseItem.ui \
    $$PWD/plugins/autoresponse/SAKOtherAutoResponseItemManager.ui \
    $$PWD/plugins/highlighter/SAKOtherHighlighterManager.ui \
    $$PWD/plugins/timingsent/SAKOtherTimingSentItem.ui \
    $$PWD/plugins/timingsent/SAKOtherTimingSentItemManager.ui \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemCom.ui \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemTcp.ui \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemUdp.ui \
    $$PWD/plugins/transmission/SAKOtherTransmissionPage.ui \
    $$PWD/plugins/transmission/SAKOtherTransmissionPageViewer.ui \
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
    $$PWD/input/datafactory/SAKInputDataFactory.hh \
    $$PWD/input/datapreset/SAKInputDataPresetItem.hh \
    $$PWD/input/datapreset/SAKInputDataPresetItemManager.hh \
    $$PWD/plugins/SAKDebugPageOtherController.hh \
    $$PWD/device/analyzer/SAKOtherAnalyzerThread.hh \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.hh \
    $$PWD/plugins/autoresponse/SAKOtherAutoResponseItem.hh \
    $$PWD/plugins/autoresponse/SAKOtherAutoResponseItemManager.hh \
    $$PWD/plugins/highlighter/SAKOtherHighlighter.hh \
    $$PWD/plugins/highlighter/SAKOtherHighlighterManager.hh \
    $$PWD/plugins/timingsent/SAKOtherTimingSentItem.hh \
    $$PWD/plugins/timingsent/SAKOtherTimingSentItemManager.hh \
    $$PWD/plugins/transmission/SAKOtherTransmissionItem.hh \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemCom.hh \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemTcp.hh \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemUdp.hh \
    $$PWD/plugins/transmission/SAKOtherTransmissionPage.hh \
    $$PWD/plugins/transmission/SAKOtherTransmissionPageViewer.hh \
    $$PWD/output/SAKDebuggerOutput.hh \
    $$PWD/output/SAKDebuggerOutputHighlighter.hh \
    $$PWD/output/SAKDebuggerOutputLog.hh \
    $$PWD/output/SAKDebuggerOutputSave2File.hh \
    $$PWD/plugins/SAKDebuggerPlugins.hh \
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
    $$PWD/input/datafactory/SAKInputDataFactory.cc \
    $$PWD/input/datapreset/SAKInputDataPresetItem.cc \
    $$PWD/input/datapreset/SAKInputDataPresetItemManager.cc \
    $$PWD/plugins/SAKDebugPageOtherController.cc \
    $$PWD/device/analyzer/SAKOtherAnalyzerThread.cc \
    $$PWD/device/analyzer/SAKOtherAnalyzerThreadManager.cc \
    $$PWD/plugins/autoresponse/SAKOtherAutoResponseItem.cc \
    $$PWD/plugins/autoresponse/SAKOtherAutoResponseItemManager.cc \
    $$PWD/plugins/highlighter/SAKOtherHighlighter.cc \
    $$PWD/plugins/highlighter/SAKOtherHighlighterManager.cc \
    $$PWD/plugins/timingsent/SAKOtherTimingSentItem.cc \
    $$PWD/plugins/timingsent/SAKOtherTimingSentItemManager.cc \
    $$PWD/plugins/transmission/SAKOtherTransmissionItem.cc \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemCom.cc \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemTcp.cc \
    $$PWD/plugins/transmission/SAKOtherTransmissionItemUdp.cc \
    $$PWD/plugins/transmission/SAKOtherTransmissionPage.cc \
    $$PWD/plugins/transmission/SAKOtherTransmissionPageViewer.cc \
    $$PWD/output/SAKDebuggerOutput.cc \
    $$PWD/output/SAKDebuggerOutputHighlighter.cc \
    $$PWD/output/SAKDebuggerOutputLog.cc \
    $$PWD/output/SAKDebuggerOutputSave2File.cc \
    $$PWD/plugins/SAKDebuggerPlugins.cc \
    $$PWD/statistics/SAKDebuggerStatistics.cc
