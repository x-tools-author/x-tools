/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#include <QTabWidget>
#include <QTimer>
#include <QToolButton>
#include <QVariantMap>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Page;
}
QT_END_NAMESPACE

class Preset;
class Emitter;
class Responder;
#ifdef X_ENABLE_SERIAL_PORT
class SerialPortTransfer;
class SerialPortTransferUi;
#endif
class UdpClientTransfer;
class UdpClientTransferUi;
class UdpServerTransfer;
class UdpServerTransferUi;
class TcpClientTransfer;
class TcpClientTransferUi;
class TcpServerTransfer;
class TcpServerTransferUi;
#ifdef X_ENABLE_WEB_SOCKET
class WebSocketClientTransfer;
class WebSocketClientTransferUi;
class WebSocketServerTransfer;
class WebSocketServerTransferUi;
#endif
#ifdef X_ENABLE_CHARTS
class Charts;
class ChartsUi;
#endif

class InputSettings;
class OutputSettings;
class SyntaxHighlighter;
class DeviceSettings;

class AbstractIO;
class Statistician;
class Device;
class DeviceUi;

class Page : public QWidget
{
    Q_OBJECT
public:
    enum ControllerDirection { Left, Right };

public:
    explicit Page(ControllerDirection direction, QSettings *settings, QWidget *parent = nullptr);
    ~Page() override;

    QVariantMap save();
    void load(const QVariantMap &parameters);
    QTabWidget *tabWidget();
    QToolButton *presetToolButton();
    void inputBytes(const QByteArray &bytes);
    void prependOutputControl(QWidget *widget);
    void appendOutputControl(QWidget *widget);

signals:
    void bytesWritten(const QByteArray &bytes, const QString &to);
    void bytesRead(const QByteArray &bytes, const QString &from);

private:
    void initUi();
    void initUiDeviceControl();
    void initUiOutputControl();
    void initUiInputControl();
    void initUiOutput();
    void initUiInput();

    void onDeviceTypeChanged();
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

    void openDevice();
    void closeDevice();
    void setupDevice(Device *device);
    void writeBytes();
    void updateLabelInfo();
    void setupMenu(QPushButton *target, QWidget *actionWidget);
    void setUiEnabled(bool enabled);
    void outputText(const QByteArray &bytes, const QString &flag, bool isRx);
    void saveControllerParameters();
    void loadControllerParameters();

    QByteArray payload() const;
    QByteArray crc(const QByteArray &payload) const;
    DeviceUi *newDeviceUi(int type);

private:
    Ui::Page *ui;
    DeviceUi *m_deviceController;
    DeviceSettings *m_ioSettings;
    OutputSettings *m_outputSettings;
    InputSettings *m_inputSettings;
    QTimer *m_writeTimer;
    QTimer *m_updateLabelInfoTimer;
    SyntaxHighlighter *m_highlighter;
    Statistician *m_rxStatistician;
    Statistician *m_txStatistician;
    Preset *m_preset;
    Emitter *m_emitter;
    Responder *m_responder;
    QList<AbstractIO *> m_ioList;
    QSettings *m_settings;

    UdpClientTransfer *m_udpClientTransfer;
    UdpClientTransferUi *m_udpClientTransferUi;
    UdpServerTransfer *m_udpServerTransfer;
    UdpServerTransferUi *m_udpServerTransferUi;
    TcpClientTransfer *m_tcpClientTransfer;
    TcpClientTransferUi *m_tcpClientTransferUi;
    TcpServerTransfer *m_tcpServerTransfer;
    TcpServerTransferUi *m_tcpServerTransferUi;
#ifdef X_ENABLE_SERIAL_PORT
    SerialPortTransfer *m_serialPortTransfer;
    SerialPortTransferUi *m_serialPortTransferUi;
#endif
#ifdef X_ENABLE_WEB_SOCKET
    WebSocketClientTransfer *m_wsClientTransfer;
    WebSocketClientTransferUi *m_wsClientTransferUi;
    WebSocketServerTransfer *m_wsServerTransfer;
    WebSocketServerTransferUi *m_wsServerTransferUi;
#endif
#ifdef X_ENABLE_CHARTS
    Charts *m_charts;
    ChartsUi *m_chartsUi;
#endif
};
