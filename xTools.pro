QT       += core gui serialport network websockets svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# disables all the APIs deprecated before Qt 6.0.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$[QT_INSTALL_LIBS]
DEFINES += X_TOOLS_BUILD_WITH_QMAKE
DEFINES += X_TOOLS_ENABLE_MODULE_SERIALPORT

#---------------------------------------------------------------------------------------------------
# Application information
X_TOOLS_APP_NAME        = "xTools"
X_TOOLS_ORG_NAME        = "xTools"
X_TOOLS_ORG_DOMAIN      = "IT"
X_TOOLS_APP_DESCRIPTION = "xTools Studio"
X_TOOLS_APP_COPYRIGHT   = "Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved."
X_TOOLS_VERSION         = "6.0.0"

win32 {
    QMAKE_TARGET_COMPANY        = "$${X_TOOLS_ORG_NAME}"
    QMAKE_TARGET_DESCRIPTION    = "$${X_TOOLS_APP_DESCRIPTION}"
    QMAKE_TARGET_COPYRIGHT      = "$${X_TOOLS_APP_COPYRIGHT}"
    QMAKE_TARGET_PRODUCT        = "$${X_TOOLS_APP_NAME}"
    QMAKE_TARGET_VERSION        = "$${X_TOOLS_VERSION}"
}

#---------------------------------------------------------------------------------------------------
# Definitions
DEFINES += X_TOOLS_CLEAR_MESSAGE_INTERVAL=8000
DEFINES += X_TOOLS_VERSION=\"\\\"5.0.0\\\"\"
DEFINES += X_TOOLS_EDITION=\"\\\"Beta\\\"\"
DEFINES += X_TOOLS_AUTHOR=\"\\\"x-tools-author\\\"\"
DEFINES += X_TOOLS_AUTHOR_EMAIL=\"\\\"x-tools@outlook.com\\\"\"
DEFINES += X_TOOLS_DEFAULT_APP_STYLE=\"\\\"Fusion\\\"\"
DEFINES += X_TOOLS_GITHUB_REPOSITORY_URL=\"\\\"https://github.com/x-tools-author/x-tools\\\"\"
DEFINES += X_TOOLS_GITEE_REPOSITORY_URL=\"\\\"https://gitee.com/x-tools-author/x-tools\\\"\"
equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 13) {
    DEFINES += X_TOOLS_ENABLE_HIGH_DPI_POLICY
}
equals(QT_MAJOR_VERSION, 6) {
    DEFINES += X_TOOLS_ENABLE_HIGH_DPI_POLICY
}

exists (./.git) {
    GIT_COMMIT   = $$system(git rev-parse HEAD)
    GIT_TIME     = $$system(git log -1 --format="%cd")
    DEFINES += X_TOOLS_GIT_COMMIT=\"\\\"$${GIT_COMMIT}\\\"\"
    DEFINES += X_TOOLS_GIT_COMMIT_TIME=\"\\\"$${GIT_TIME}\\\"\"
}

#---------------------------------------------------------------------------------------------------
# Output directory
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj

#---------------------------------------------------------------------------------------------------
# Configuration of Windows
win32 {
    RC_ICONS = xTools.ico
    msvc:lessThan(QT_MAJOR_VERSION, 6) {
        QMAKE_CXXFLAGS += -execution-charset:utf-8
    }
    mingw: {
        DEFINES+=FEATURE_schannel
    }
}

#---------------------------------------------------------------------------------------------------
# Deploy Qt
contains(CONFIG, static) {
    # Static compilation does not require deployment
} else {
    # Windows platform(only for x86 architecture)
    win32 {
        DEPLOY_TOOL = $${dirname(QMAKE_QMAKE)}/windeployqt.exe
        DEPLOY_TOOL = $$replace(DEPLOY_TOOL, /, \\)

        contains(CONFIG, debug, debug|release){
            DEPLOY_TARGET = $${OUT_PWD}/debug/$${TARGET}.exe
        } else {
            DEPLOY_TARGET = $${OUT_PWD}/release/$${TARGET}.exe
        }

        DEPLOY_TARGET = $$replace(DEPLOY_TARGET, /, \\)
        msvc {
            QMAKE_POST_LINK += $${DEPLOY_TOOL} $${DEPLOY_TARGET} --force --no-translations $$escape_expand(\\n)
        } else {
            QMAKE_POST_LINK += '$$escape_expand("\\n\\t") $${DEPLOY_TOOL} $${DEPLOY_TARGET} --force --no-translations $$escape_expand("\\n\\t")'
        }
    }
}

#---------------------------------------------------------------------------------------------------
# Include path
INCLUDEPATH += Source
INCLUDEPATH += Source/Assistants
INCLUDEPATH += Source/Assistants/ASCII/Source
INCLUDEPATH += Source/Assistants/Base64/Source
INCLUDEPATH += Source/Assistants/Broadcast/Source
INCLUDEPATH += Source/Assistants/CRC/Source
INCLUDEPATH += Source/Assistants/FileCheck/Source
INCLUDEPATH += Source/Assistants/Number/Source
INCLUDEPATH += Source/Assistants/Ping/Source
INCLUDEPATH += Source/Assistants/String/Source
INCLUDEPATH += Source/Assistants/FileMerge/Source
INCLUDEPATH += Source/Common/Common
INCLUDEPATH += Source/Common/CommonUI
INCLUDEPATH += Source/Tools/Tools
INCLUDEPATH += Source/Tools/ToolsUI
INCLUDEPATH += Source/ToolBox/ToolBox
INCLUDEPATH += Source/ToolBox/ToolBoxUI
INCLUDEPATH += Source/ToolBox/ToolBox
INCLUDEPATH += Source/ToolBox/ToolBoxUI

#---------------------------------------------------------------------------------------------------
# Source of project
RESOURCES += \
    xTools.qrc \
    Source/Common/xToolsCommon.qrc

SOURCES += \
    Source/Application.cpp \
    Source/Assistants/ASCII/Source/xToolsAsciiAssistant.cpp \
    Source/Assistants/Base64/Source/xToolsBase64Assisatnt.cpp \
    Source/Assistants/Broadcast/Source/xToolsBroadcastAssistant.cpp \
    Source/Assistants/Broadcast/Source/xToolsBroadcastThread.cpp \
    Source/Assistants/CRC/Source/xToolsCRCAssistant.cpp \
    Source/Assistants/FileCheck/Source/xToolsCryptographicHashCalculator.cpp \
    Source/Assistants/FileCheck/Source/xToolsFileCheckAssistant.cpp \
    Source/Assistants/FileMerge/Source/xToolsFileMergeAssistant.cpp \
    Source/Assistants/Number/Source/xToolsNumberAssistant.cpp \
    Source/Assistants/Ping/Source/xToolsPingAssistant.cpp \
    Source/Assistants/String/Source/xToolsStringAssistant.cpp \
    Source/Assistants/xToolsAssistantFactory.cpp \
    Source/Common/Common/xToolsApplication.cpp \
    Source/Common/Common/xToolsCrcInterface.cpp \
    Source/Common/Common/xToolsDataStructure.cpp \
    Source/Common/Common/xToolsNetworkInterfaceScanner.cpp \
    Source/Common/Common/xToolsSerialPortScanner.cpp \
    Source/Common/Common/xToolsSettings.cpp \
    Source/Common/Common/xToolsTableModel.cpp \
    Source/Common/CommonUI/xToolsAffixesComboBox.cpp \
    Source/Common/CommonUI/xToolsBaudRateComboBox.cpp \
    Source/Common/CommonUI/xToolsCheckBox.cpp \
    Source/Common/CommonUI/xToolsComboBox.cpp \
    Source/Common/CommonUI/xToolsCrcAlgorithmComboBox.cpp \
    Source/Common/CommonUI/xToolsDataBitsComboBox.cpp \
    Source/Common/CommonUI/xToolsEscapeCharacterComboBox.cpp \
    Source/Common/CommonUI/xToolsFlowControlComboBox.cpp \
    Source/Common/CommonUI/xToolsHighlighter.cpp \
    Source/Common/CommonUI/xToolsIpComboBox.cpp \
    Source/Common/CommonUI/xToolsLineEdit.cpp \
    Source/Common/CommonUI/xToolsMainWindow.cpp \
    Source/Common/CommonUI/xToolsMenu.cpp \
    Source/Common/CommonUI/xToolsParityComboBox.cpp \
    Source/Common/CommonUI/xToolsPortNameComboBox.cpp \
    Source/Common/CommonUI/xToolsResponseOptionComboBox.cpp \
    Source/Common/CommonUI/xToolsSpinBox.cpp \
    Source/Common/CommonUI/xToolsStopBitsComboBox.cpp \
    Source/Common/CommonUI/xToolsTextFormatComboBox.cpp \
    Source/Common/CommonUI/xToolsWebSocketMessageTypeComboBox.cpp \
    Source/SystemTrayIcon.cpp \
    Source/ToolBox/ToolBox/xToolsToolBox.cpp \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUi.cpp \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiCommunicationMenu.cpp \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiInputMenu.cpp \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiOutputMenu.cpp \
    Source/Tools/Tools/xToolsAnalyzerTool.cpp \
    Source/Tools/Tools/xToolsBaseTool.cpp \
    Source/Tools/Tools/xToolsCommunicationTool.cpp \
    Source/Tools/Tools/xToolsCrcCalculatorTool.cpp \
    Source/Tools/Tools/xToolsEmitterTool.cpp \
    Source/Tools/Tools/xToolsMaskerTool.cpp \
    Source/Tools/Tools/xToolsPrestorerTool.cpp \
    Source/Tools/Tools/xToolsResponserTool.cpp \
    Source/Tools/Tools/xToolsSerialPortTool.cpp \
    Source/Tools/Tools/xToolsSerialPortTransmitterTool.cpp \
    Source/Tools/Tools/xToolsSocketClientTool.cpp \
    Source/Tools/Tools/xToolsSocketClientTransmitterTool.cpp \
    Source/Tools/Tools/xToolsSocketServerTool.cpp \
    Source/Tools/Tools/xToolsStatisticianTool.cpp \
    Source/Tools/Tools/xToolsStorerTool.cpp \
    Source/Tools/Tools/xToolsTableModelTool.cpp \
    Source/Tools/Tools/xToolsTcpClientTool.cpp \
    Source/Tools/Tools/xToolsTcpServerTool.cpp \
    Source/Tools/Tools/xToolsTcpTransmitterTool.cpp \
    Source/Tools/Tools/xToolsToolFactory.cpp \
    Source/Tools/Tools/xToolsTransmitterTool.cpp \
    Source/Tools/Tools/xToolsUdpClientTool.cpp \
    Source/Tools/Tools/xToolsUdpServerTool.cpp \
    Source/Tools/Tools/xToolsUdpTransmitterTool.cpp \
    Source/Tools/Tools/xToolsVelometerTool.cpp \
    Source/Tools/Tools/xToolsWebSocketClientTool.cpp \
    Source/Tools/Tools/xToolsWebSocketServerTool.cpp \
    Source/Tools/Tools/xToolsWebSocketTransmitterTool.cpp \
    Source/Tools/ToolsUI/xToolsAnalyzerToolUi.cpp \
    Source/Tools/ToolsUI/xToolsBaseToolUi.cpp \
    Source/Tools/ToolsUI/xToolsCommunicationToolUi.cpp \
    Source/Tools/ToolsUI/xToolsCrcCalculatorToolUi.cpp \
    Source/Tools/ToolsUI/xToolsEmitterToolUi.cpp \
    Source/Tools/ToolsUI/xToolsEmitterToolUiEditor.cpp \
    Source/Tools/ToolsUI/xToolsMaskerToolUi.cpp \
    Source/Tools/ToolsUI/xToolsPrestorerToolUi.cpp \
    Source/Tools/ToolsUI/xToolsPrestorerToolUiEditor.cpp \
    Source/Tools/ToolsUI/xToolsResponserToolUi.cpp \
    Source/Tools/ToolsUI/xToolsResponserToolUiEditor.cpp \
    Source/Tools/ToolsUI/xToolsSerialPortToolUi.cpp \
    Source/Tools/ToolsUI/xToolsSerialPortTransmitterToolUi.cpp \
    Source/Tools/ToolsUI/xToolsSerialPortTransmitterToolUiEditor.cpp \
    Source/Tools/ToolsUI/xToolsSocketClientToolUi.cpp \
    Source/Tools/ToolsUI/xToolsSocketClientTransmitterToolUi.cpp \
    Source/Tools/ToolsUI/xToolsSocketClientTransmitterToolUiEditor.cpp \
    Source/Tools/ToolsUI/xToolsSocketServerToolUi.cpp \
    Source/Tools/ToolsUI/xToolsStatisticianToolUi.cpp \
    Source/Tools/ToolsUI/xToolsStorerToolUi.cpp \
    Source/Tools/ToolsUI/xToolsTableModelToolUi.cpp \
    Source/Tools/ToolsUI/xToolsTableViewToolUi.cpp \
    Source/Tools/ToolsUI/xToolsTcpTransmitterToolUi.cpp \
    Source/Tools/ToolsUI/xToolsToolUiFactory.cpp \
    Source/Tools/ToolsUI/xToolsTransmitterToolUi.cpp \
    Source/Tools/ToolsUI/xToolsUdpTransmitterToolUi.cpp \
    Source/Tools/ToolsUI/xToolsVelometerToolUi.cpp \
    Source/Tools/ToolsUI/xToolsWebSocketTransmitterToolUi.cpp \
    Source/main.cpp \
    Source/MainWindow.cpp

HEADERS += \
    Source/Application.h \
    Source/Assistants/ASCII/Source/xToolsAsciiAssistant.h \
    Source/Assistants/Base64/Source/xToolsBase64Assisatnt.h \
    Source/Assistants/Broadcast/Source/xToolsBroadcastAssistant.h \
    Source/Assistants/Broadcast/Source/xToolsBroadcastThread.h \
    Source/Assistants/CRC/Source/xToolsCRCAssistant.h \
    Source/Assistants/FileCheck/Source/xToolsCryptographicHashCalculator.h \
    Source/Assistants/FileCheck/Source/xToolsFileCheckAssistant.h \
    Source/Assistants/FileMerge/Source/xToolsFileMergeAssistant.h \
    Source/Assistants/Number/Source/xToolsNumberAssistant.h \
    Source/Assistants/Ping/Source/xToolsPingAssistant.h \
    Source/Assistants/String/Source/xToolsStringAssistant.h \
    Source/Assistants/xToolsAssistantFactory.h \
    Source/Common/Common/xToolsApplication.h \
    Source/Common/Common/xToolsCompatibility.h \
    Source/Common/Common/xToolsCrcInterface.h \
    Source/Common/Common/xToolsDataStructure.h \
    Source/Common/Common/xToolsNetworkInterfaceScanner.h \
    Source/Common/Common/xToolsSerialPortScanner.h \
    Source/Common/Common/xToolsSettings.h \
    Source/Common/Common/xToolsTableModel.h \
    Source/Common/CommonUI/xTools.h \
    Source/Common/CommonUI/xToolsAffixesComboBox.h \
    Source/Common/CommonUI/xToolsBaudRateComboBox.h \
    Source/Common/CommonUI/xToolsCheckBox.h \
    Source/Common/CommonUI/xToolsComboBox.h \
    Source/Common/CommonUI/xToolsCrcAlgorithmComboBox.h \
    Source/Common/CommonUI/xToolsDataBitsComboBox.h \
    Source/Common/CommonUI/xToolsEscapeCharacterComboBox.h \
    Source/Common/CommonUI/xToolsFlowControlComboBox.h \
    Source/Common/CommonUI/xToolsHighlighter.h \
    Source/Common/CommonUI/xToolsIpComboBox.h \
    Source/Common/CommonUI/xToolsLineEdit.h \
    Source/Common/CommonUI/xToolsMainWindow.h \
    Source/Common/CommonUI/xToolsMenu.h \
    Source/Common/CommonUI/xToolsParityComboBox.h \
    Source/Common/CommonUI/xToolsPortNameComboBox.h \
    Source/Common/CommonUI/xToolsResponseOptionComboBox.h \
    Source/Common/CommonUI/xToolsSpinBox.h \
    Source/Common/CommonUI/xToolsStopBitsComboBox.h \
    Source/Common/CommonUI/xToolsTextFormatComboBox.h \
    Source/Common/CommonUI/xToolsWebSocketMessageTypeComboBox.h \
    Source/MainWindow.h \
    Source/SystemTrayIcon.h \
    Source/ToolBox/ToolBox/xToolsToolBox.h \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUi.h \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiCommunicationMenu.h \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiInputMenu.h \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiOutputMenu.h \
    Source/Tools/Tools/xToolsAnalyzerTool.h \
    Source/Tools/Tools/xToolsBaseTool.h \
    Source/Tools/Tools/xToolsCommunicationTool.h \
    Source/Tools/Tools/xToolsCrcCalculatorTool.h \
    Source/Tools/Tools/xToolsEmitterTool.h \
    Source/Tools/Tools/xToolsMaskerTool.h \
    Source/Tools/Tools/xToolsPrestorerTool.h \
    Source/Tools/Tools/xToolsResponserTool.h \
    Source/Tools/Tools/xToolsSerialPortTool.h \
    Source/Tools/Tools/xToolsSerialPortTransmitterTool.h \
    Source/Tools/Tools/xToolsSocketClientTool.h \
    Source/Tools/Tools/xToolsSocketClientTransmitterTool.h \
    Source/Tools/Tools/xToolsSocketServerTool.h \
    Source/Tools/Tools/xToolsStatisticianTool.h \
    Source/Tools/Tools/xToolsStorerTool.h \
    Source/Tools/Tools/xToolsTableModelTool.h \
    Source/Tools/Tools/xToolsTcpClientTool.h \
    Source/Tools/Tools/xToolsTcpServerTool.h \
    Source/Tools/Tools/xToolsTcpTransmitterTool.h \
    Source/Tools/Tools/xToolsToolFactory.h \
    Source/Tools/Tools/xToolsTransmitterTool.h \
    Source/Tools/Tools/xToolsUdpClientTool.h \
    Source/Tools/Tools/xToolsUdpServerTool.h \
    Source/Tools/Tools/xToolsUdpTransmitterTool.h \
    Source/Tools/Tools/xToolsVelometerTool.h \
    Source/Tools/Tools/xToolsWebSocketClientTool.h \
    Source/Tools/Tools/xToolsWebSocketServerTool.h \
    Source/Tools/Tools/xToolsWebSocketTransmitterTool.h \
    Source/Tools/ToolsUI/xToolsAnalyzerToolUi.h \
    Source/Tools/ToolsUI/xToolsBaseToolUi.h \
    Source/Tools/ToolsUI/xToolsCommunicationToolUi.h \
    Source/Tools/ToolsUI/xToolsCrcCalculatorToolUi.h \
    Source/Tools/ToolsUI/xToolsEmitterToolUi.h \
    Source/Tools/ToolsUI/xToolsEmitterToolUiEditor.h \
    Source/Tools/ToolsUI/xToolsMaskerToolUi.h \
    Source/Tools/ToolsUI/xToolsPrestorerToolUi.h \
    Source/Tools/ToolsUI/xToolsPrestorerToolUiEditor.h \
    Source/Tools/ToolsUI/xToolsResponserToolUi.h \
    Source/Tools/ToolsUI/xToolsResponserToolUiEditor.h \
    Source/Tools/ToolsUI/xToolsSerialPortToolUi.h \
    Source/Tools/ToolsUI/xToolsSerialPortTransmitterToolUi.h \
    Source/Tools/ToolsUI/xToolsSerialPortTransmitterToolUiEditor.h \
    Source/Tools/ToolsUI/xToolsSocketClientToolUi.h \
    Source/Tools/ToolsUI/xToolsSocketClientTransmitterToolUi.h \
    Source/Tools/ToolsUI/xToolsSocketClientTransmitterToolUiEditor.h \
    Source/Tools/ToolsUI/xToolsSocketServerToolUi.h \
    Source/Tools/ToolsUI/xToolsStatisticianToolUi.h \
    Source/Tools/ToolsUI/xToolsStorerToolUi.h \
    Source/Tools/ToolsUI/xToolsTableModelToolUi.h \
    Source/Tools/ToolsUI/xToolsTableViewToolUi.h \
    Source/Tools/ToolsUI/xToolsTcpTransmitterToolUi.h \
    Source/Tools/ToolsUI/xToolsToolUiFactory.h \
    Source/Tools/ToolsUI/xToolsTransmitterToolUi.h \
    Source/Tools/ToolsUI/xToolsUdpTransmitterToolUi.h \
    Source/Tools/ToolsUI/xToolsVelometerToolUi.h \
    Source/Tools/ToolsUI/xToolsWebSocketTransmitterToolUi.h

FORMS += \
    Source/Assistants/ASCII/Source/xToolsAsciiAssistant.ui \
    Source/Assistants/Base64/Source/xToolsBase64Assisatnt.ui \
    Source/Assistants/Broadcast/Source/xToolsBroadcastAssistant.ui \
    Source/Assistants/CRC/Source/xToolsCRCAssistant.ui \
    Source/Assistants/FileCheck/Source/xToolsFileCheckAssistant.ui \
    Source/Assistants/FileMerge/Source/xToolsFileMergeAssistant.ui \
    Source/Assistants/Number/Source/xToolsNumberAssistant.ui \
    Source/Assistants/Ping/Source/xToolsPingAssistant.ui \
    Source/Assistants/String/Source/xToolsStringAssistant.ui \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUi.ui \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiCommunicationMenu.ui \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiInputMenu.ui \
    Source/ToolBox/ToolBoxUI/xToolsToolBoxUiOutputMenu.ui \
    Source/Tools/ToolsUI/xToolsAnalyzerToolUi.ui \
    Source/Tools/ToolsUI/xToolsCrcCalculatorToolUi.ui \
    Source/Tools/ToolsUI/xToolsEmitterToolUi.ui \
    Source/Tools/ToolsUI/xToolsEmitterToolUiEditor.ui \
    Source/Tools/ToolsUI/xToolsMaskerToolUi.ui \
    Source/Tools/ToolsUI/xToolsPrestorerToolUiEditor.ui \
    Source/Tools/ToolsUI/xToolsResponserToolUiEditor.ui \
    Source/Tools/ToolsUI/xToolsSerialPortToolUi.ui \
    Source/Tools/ToolsUI/xToolsSerialPortTransmitterToolUiEditor.ui \
    Source/Tools/ToolsUI/xToolsSocketClientToolUi.ui \
    Source/Tools/ToolsUI/xToolsSocketClientTransmitterToolUiEditor.ui \
    Source/Tools/ToolsUI/xToolsSocketServerToolUi.ui \
    Source/Tools/ToolsUI/xToolsStatisticianToolUi.ui \
    Source/Tools/ToolsUI/xToolsStorerToolUi.ui \
    Source/Tools/ToolsUI/xToolsTableModelToolUi.ui \
    Source/Tools/ToolsUI/xToolsVelometerToolUi.ui
