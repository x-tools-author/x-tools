include($$PWD/charts/SAKCharts.pri)

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
    $$PWD/other \
    $$PWD/other/autoresponse \
    $$PWD/other/highlighter \
    $$PWD/other/generator \
    $$PWD/other/analyzer \
    $$PWD/other/timingsent \
    $$PWD/other/transmission \
    $$PWD/output \
    $$PWD/statistics

FORMS += \
    $$PWD/SAKDebugger.ui \
    $$PWD/common/SAKDebugPageCommonSslConfigurationWidget.ui \
    $$PWD/device/mask/SAKDebugPageDeviceMask.ui \
    $$PWD/input/crcsettings/SAKInputCrcSettingsDialog.ui \
    $$PWD/input/datapreset/SAKInputDataPresetItem.ui \
    $$PWD/input/datapreset/SAKInputDataPresetItemManager.ui \
    $$PWD/other/analyzer/SAKOtherAnalyzerThreadManager.ui \
    $$PWD/other/autoresponse/SAKOtherAutoResponseItem.ui \
    $$PWD/other/autoresponse/SAKOtherAutoResponseItemManager.ui \
    $$PWD/other/highlighter/SAKOtherHighlighterManager.ui \
    $$PWD/other/timingsent/SAKOtherTimingSentItem.ui \
    $$PWD/other/timingsent/SAKOtherTimingSentItemManager.ui \
    $$PWD/other/transmission/SAKOtherTransmissionItemCom.ui \
    $$PWD/other/transmission/SAKOtherTransmissionItemTcp.ui \
    $$PWD/other/transmission/SAKOtherTransmissionItemUdp.ui \
    $$PWD/other/transmission/SAKOtherTransmissionPage.ui \
    $$PWD/other/transmission/SAKOtherTransmissionPageViewer.ui \
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
    $$PWD/other/SAKDebugPageOtherController.hh \
    $$PWD/other/analyzer/SAKOtherAnalyzerThread.hh \
    $$PWD/other/analyzer/SAKOtherAnalyzerThreadManager.hh \
    $$PWD/other/autoresponse/SAKOtherAutoResponseItem.hh \
    $$PWD/other/autoresponse/SAKOtherAutoResponseItemManager.hh \
    $$PWD/other/highlighter/SAKOtherHighlighter.hh \
    $$PWD/other/highlighter/SAKOtherHighlighterManager.hh \
    $$PWD/other/timingsent/SAKOtherTimingSentItem.hh \
    $$PWD/other/timingsent/SAKOtherTimingSentItemManager.hh \
    $$PWD/other/transmission/SAKOtherTransmissionItem.hh \
    $$PWD/other/transmission/SAKOtherTransmissionItemCom.hh \
    $$PWD/other/transmission/SAKOtherTransmissionItemTcp.hh \
    $$PWD/other/transmission/SAKOtherTransmissionItemUdp.hh \
    $$PWD/other/transmission/SAKOtherTransmissionPage.hh \
    $$PWD/other/transmission/SAKOtherTransmissionPageViewer.hh \
    $$PWD/output/SAKDebuggerOutput.hh \
    $$PWD/output/SAKDebuggerOutputHighlighter.hh \
    $$PWD/output/SAKDebuggerOutputLog.hh \
    $$PWD/output/SAKDebuggerOutputSave2File.hh \
    $$PWD/statistics/SAKDebugPageStatisticsController.hh

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
    $$PWD/other/SAKDebugPageOtherController.cc \
    $$PWD/other/analyzer/SAKOtherAnalyzerThread.cc \
    $$PWD/other/analyzer/SAKOtherAnalyzerThreadManager.cc \
    $$PWD/other/autoresponse/SAKOtherAutoResponseItem.cc \
    $$PWD/other/autoresponse/SAKOtherAutoResponseItemManager.cc \
    $$PWD/other/highlighter/SAKOtherHighlighter.cc \
    $$PWD/other/highlighter/SAKOtherHighlighterManager.cc \
    $$PWD/other/timingsent/SAKOtherTimingSentItem.cc \
    $$PWD/other/timingsent/SAKOtherTimingSentItemManager.cc \
    $$PWD/other/transmission/SAKOtherTransmissionItem.cc \
    $$PWD/other/transmission/SAKOtherTransmissionItemCom.cc \
    $$PWD/other/transmission/SAKOtherTransmissionItemTcp.cc \
    $$PWD/other/transmission/SAKOtherTransmissionItemUdp.cc \
    $$PWD/other/transmission/SAKOtherTransmissionPage.cc \
    $$PWD/other/transmission/SAKOtherTransmissionPageViewer.cc \
    $$PWD/output/SAKDebuggerOutput.cc \
    $$PWD/output/SAKDebuggerOutputHighlighter.cc \
    $$PWD/output/SAKDebuggerOutputLog.cc \
    $$PWD/output/SAKDebuggerOutputSave2File.cc \
    $$PWD/statistics/SAKDebugPageStatisticsController.cc
