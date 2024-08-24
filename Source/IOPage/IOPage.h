/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QButtonGroup>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include <QVariantMap>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class IOPage;
}
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace xTools {
class Preset;
class Emitter;
class Responser;
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
class SerialPortTransfer;
class SerialPortTransferUi;
#endif
class UdpClientTransfer;
class UdpClientTransferUi;
class TcpClientTransfer;
class TcpClientTransferUi;
class TcpServerTransfer;
class TcpServerTransferUi;
class WebSocketClientTransfer;
class WebSocketClientTransferUi;
class WebSocketServerTransfer;
class WebSocketServerTransferUi;
}
QT_END_NAMESPACE

class InputSettings;
class OutputSettings;
class SyntaxHighlighter;
class CommunicationSettings;

namespace xTools {
class AbstractIO;
class Statistician;
class Communication;
class CommunicationUi;
} // namespace xTools

class IOPage : public QWidget
{
    Q_OBJECT
public:
    enum ControllerDirection { Left, Right };

public:
    explicit IOPage(ControllerDirection direction, QSettings *settings, QWidget *parent = nullptr);
    ~IOPage() override;

    QVariantMap save();
    void load(const QVariantMap &parameters);

private:
    Ui::IOPage *ui;
    xTools::Communication *m_io;
    xTools::CommunicationUi *m_ioUi;
    CommunicationSettings *m_ioSettings;
    OutputSettings *m_outputSettings;
    InputSettings *m_inputSettings;
    QTimer *m_writeTimer;
    QTimer *m_updateLabelInfoTimer;
    SyntaxHighlighter *m_highlighter;
    xTools::Statistician *m_rxStatistician;
    xTools::Statistician *m_txStatistician;
    xTools::Preset *m_preset;
    xTools::Emitter *m_emitter;
    xTools::Responser *m_responder;
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    xTools::SerialPortTransfer *m_serialPortTransfer;
    xTools::SerialPortTransferUi *m_serialPortTransferUi;
#endif
    xTools::UdpClientTransfer *m_udpClientTransfer;
    xTools::UdpClientTransferUi *m_udpClientTransferUi;
    xTools::TcpClientTransfer *m_tcpClientTransfer;
    xTools::TcpClientTransferUi *m_tcpClientTransferUi;
    xTools::TcpServerTransfer *m_tcpServerTransfer;
    xTools::TcpServerTransferUi *m_tcpServerTransferUi;
    xTools::WebSocketClientTransfer *m_webSocketClientTransfer;
    xTools::WebSocketClientTransferUi *m_webSocketClientTransferUi;
    xTools::WebSocketServerTransfer *m_webSocketServerTransfer;
    xTools::WebSocketServerTransferUi *m_webSocketServerTransferUi;

    QButtonGroup m_pageButtonGroup;
    QList<xTools::AbstractIO *> m_ioList;
    QSettings *m_settings;

private:
    void initUi();
    void initUiCommunication();
    void initUiOutputControl();
    void initUiInputControl();
    void initUiOutput();
    void initUiInput();

    void onCommunicationTypeChanged();
    void onCycleIntervalChanged();
    void onInputFormatChanged();
    void onOpenButtonClicked();
    void onHighlighterEnableChanged();
    void onHighlighterKeywordsChanged();
    void onShowStatisticianChanged(bool checked);

    void onOpened();
    void onClosed();
    void onErrorOccurred(const QString &error);
    void onWarningOccurred(const QString &warning);
    void onBytesRead(const QByteArray &bytes, const QString &from);
    void onBytesWritten(const QByteArray &bytes, const QString &to);

    void openCommunication();
    void closeCommunication();
    void writeBytes();
    void updateLabelInfo();
    void setupMenu(QPushButton *target, QWidget *actionWidget);
    void setUiEnabled(bool enabled);
    void outputText(const QByteArray &bytes, const QString &flag, bool isRx);
    void saveControllerParameters();
    void loadControllerParameters();

    QByteArray payload() const;
    QByteArray crc(const QByteArray &payload) const;
};
