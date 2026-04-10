/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QFile>
#include <QHostAddress>
#include <QTimer>
#include <QUdpSocket>
#include <QWidget>

namespace Ui {
class TftpClient;
}

class TftpClient : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit TftpClient(QWidget* parent = Q_NULLPTR);
    ~TftpClient();

private:
    enum class TransferMode { Idle, Upload, Download };

    Ui::TftpClient* ui;
    QUdpSocket* m_socket;
    QTimer m_timeoutTimer;
    QFile m_file;
    TransferMode m_transferMode;
    QHostAddress m_serverAddress;
    quint16 m_serverPort;
    quint16 m_serverTid;
    quint16 m_currentBlock;
    quint16 m_pendingBlock;
    QByteArray m_pendingPayload;
    QByteArray m_lastPacket;
    qint64 m_totalBytes;
    qint64 m_bytesTransferred;
    int m_retryCount;
    bool m_waitingForFinalAck;
    QString m_currentRemoteFile;

private:
    void setupUiState();
    void startUpload();
    void startDownload();
    void beginTransfer(TransferMode mode);
    void finishTransfer(bool success, const QString& message);
    void abortTransfer(const QString& message);
    void resetTransfer();
    void sendPacket(const QByteArray& packet, quint16 port);
    void sendUploadBlock();
    void updateProgress();
    void setProgressIndeterminate(bool upload, bool indeterminate);
    void handleIncomingDatagram(const QByteArray& datagram,
                                const QHostAddress& senderAddress,
                                quint16 senderPort);
    bool ensureSocket();
    bool ensureDownloadPath();
    QString selectedServerAddress() const;
    QString defaultDownloadPath() const;

private slots:
    void browseUploadFile();
    void onUploadClicked();
    void onDownloadClicked();
    void onDownloadFileChanged(const QString& text);
    void onReadyRead();
    void onTimeout();
};