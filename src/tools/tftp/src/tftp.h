/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QObject>
#include <QSocketNotifier>
#include <QString>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>
#include <QUdpSocket>

#include "common/xdq.h"

enum Block {
    _RRQ = 1,   // read request
    _WRQ = 2,   // write request
    _DATA = 3,  // data packet
    _ACK = 4,   // acknowledgement
    _ERROR = 5, // error code
    _OACK = 6,
};

enum TftpError {
    EUNDEF = 0,    /* not defined */
    ENOTFOUND = 1, /* file not found */
    EACCESS = 2,   /* access violation */
    ENOSPACE = 3,  /* disk full or allocation exceeded */
    EBADOP = 4,    /* illegal TFTP operation */
    EBADID = 5,    /* unknown transfer ID */
    EEXISTS = 6,   /* file already exists */
    ENOUSER = 7    /* no such user */
};

#define OCTET 1
#define BLOCK_SIZE 512
#define BUFFER_SIZE 65536

class Tftp : public QThread
{
    Q_OBJECT
    X_DECLARE_PRIVATE(Tftp)
public:
    Tftp(QObject *parent = nullptr);
    ~Tftp();

    // 客户端功能相关方法
    void setClientMode(bool clientMode);
    void setDestinationAddress(const QHostAddress &address, quint16 port);
    void startDownload(const QString &remoteFileName, const QString &localFileName = "");
    void startUpload(const QString &localFileName, const QString &remoteFileName = "");

signals:
    void transferProgressSignal(QString filePath, qint64 cur, qint64 total, int speed);
    void tftpErrorSignal(QString errorMsg);
    void transferCompletedSignal(QString fileName);

protected:
    void run() override;

private:
    bool initFlag = false;
    bool dataAvalivabe = false;
    bool isClientMode = false; // 客户端模式标志
    bool isRunningFlag = true; // 线程运行标志
    QString curFilePath;
    QString pre_filePath;
    int transportType = 0;
    uint32_t file_size = 0, totalSize = 0;
    uint32_t blockSize = 0;
    uint16_t blockIndex = 1;
    QFile *file = nullptr;
    int endtftp = 0;
    int timeout = 0;
    int retryCount = 2;
    QTimer *timeoutTimer;
    char *sendBuf;
    char *sptr;
    QByteArray readBuf;
    QHostAddress destAddress; // 目标ip
    qint16 destPort = 69;     // 目标port
    int port = 69;
    QUdpSocket *udpSocket = nullptr;
    QNetworkDatagram datagram;
    int progress_bak = 0;
    int getFileSize(QString path);
    void sendErrorCode(enum TftpError code);
    void clearStatus();
    void on_recvfrom();
    void printProcess(int maxValue, int curValue);
    const QString fileMd5(QFile &sourceFile);
    const QString fileMd5(const QString &path);

    // 客户端功能相关方法
    void sendRRQ(const QString &fileName);
    void sendWRQ(const QString &fileName);
    void processClientDatagram();

public slots:
    void readPendingDatagramsSlots();
    void setWorkPathSlots(QString path);
    void setPortSlots(int port);
};