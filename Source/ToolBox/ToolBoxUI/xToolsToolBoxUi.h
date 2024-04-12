/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QIcon>
#include <QList>
#include <QTimer>
#include <QWidget>

#include "xToolsCommunicationToolUi.h"
#include "xToolsToolBox.h"

namespace Ui {
class xToolsToolBoxUi;
}

class xToolsEmitterToolUi;
class xToolsResponserToolUi;
class xToolsPrestorerToolUi;
class xToolsCommunicationTool;
class xToolsToolBoxUiInputMenu;
class xToolsToolBoxUiOutputMenu;
class xToolsCommunicationToolUi;
class xToolsTcpTransmitterToolUi;
class xToolsUdpTransmitterToolUi;
class xToolsToolBoxUiCommunicationMenu;
class xToolsWebSocketTransmitterToolUi;
class xToolsSerialPortTransmitterToolUi;
class xToolsToolBoxUi : public QWidget
{
    Q_OBJECT
public:
    explicit xToolsToolBoxUi(QWidget *parent = nullptr);
    ~xToolsToolBoxUi() override;

    static QList<int> supportedCommunicationTools();
    static QString communicationToolName(int type);
    static QIcon communicationToolIcon(int type);

    void initialize(int type);

private:
    xToolsToolBox *m_toolBox{nullptr};
    xToolsCommunicationTool *m_communication{nullptr};
    xToolsCommunicationToolUi *m_communicationUi{nullptr};
    xToolsEmitterToolUi *m_emitterUi{nullptr};
    xToolsResponserToolUi *m_responserUi{nullptr};
    xToolsPrestorerToolUi *m_prestorerUi{nullptr};
    xToolsTcpTransmitterToolUi *m_tcpTransmitterUi{nullptr};
    xToolsUdpTransmitterToolUi *m_udpTransmitterUi{nullptr};
    xToolsWebSocketTransmitterToolUi *m_webSocketTransmitterUi{nullptr};
    xToolsSerialPortTransmitterToolUi *m_serialPortTransmitterUi{nullptr};
    QTimer *m_cycleSendingTimer{nullptr};
    int m_communicationType;
    xToolsToolBoxUiCommunicationMenu *m_communicationMenu{nullptr};
    xToolsToolBoxUiInputMenu *m_inputMenu{nullptr};
    xToolsToolBoxUiOutputMenu *m_outputMenu{nullptr};

    struct
    {
        QString tabIndex;
        QString items;
        QString transmitterTabIndex;
    } m_settingsKey;

private:
    xToolsCommunicationToolUi *communicationToolUi(int type);
    void try2send();
    QString dateTimeFormat();
    void output2ui(const QByteArray &bytes, const QString &flag, bool isRx);
    QString settingsGroup();
    QByteArray calculateCrc(const QByteArray &bytes = QByteArray(), bool fixedOriginOrder = false);
    void setDefaultText();

private slots:
    void onIsWorkingChanged();
    void onBytesWritten(const QByteArray &bytes, const QString &from);
    void onBytesRead(const QByteArray &bytes, const QString &to);
    void onInputTextChanged();

private:
    void init();

    void initUi();
    void initUiCommunication();
    void initUiInput();
    void initUiOutput();

    void initSettings();
    void initSettingsCommunication();
    void initSettingsInput();
    void initSettingsOutput();

    void initSignals();
    void initSignalsCommunication();
    void initSignalsInput();
    void initSignalsOutput();

    void initTools();

private:
    Ui::xToolsToolBoxUi *ui;
    void onTabWidgetCurrentChanged(int index);

    // communication
    void onPushButtonCommunicationOpenClicked();

    // input
    void onPushButtonInputSendClicked();
    void onComboBoxInputIntervalActivated();
    void onComboBoxInputFormatActivated();
    void onComboBoxInputTextActivated(int index);

    // output
    void onCheckBoxOutputWrapClicked();
};
