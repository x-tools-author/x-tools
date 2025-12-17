/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "tftp.h"
#include <QFile>
#include <QHash>
#include <QTimer>
#include <QUdpSocket>

// TftpBase私有实现
class TftpBasePrivate
{
public:
    explicit TftpBasePrivate(TftpBase *q_ptr);
    ~TftpBasePrivate();

    void init();

public:
    QUdpSocket *udpSocket{nullptr};
    QTimer *timeoutTimer{nullptr};
    int retryCount{0};
    QByteArray lastSentData;
    QHostAddress lastSentAddress;
    quint16 lastSentPort{0};

private:
    TftpBase *q_ptr{nullptr};
};

// TftpClient私有实现
class TftpClientPrivate
{
public:
    explicit TftpClientPrivate(TftpClient *q_ptr);
    ~TftpClientPrivate();

    void init();
    void resetTransferState();

public:
    QHostAddress serverAddress;
    quint16 serverPort{DEFAULT_TFTP_PORT};
    QString localWorkPath;
    QString currentFileName;
    QFile *file{nullptr};
    qint64 fileSize{0};
    qint64 transferredBytes{0};
    quint16 blockIndex{1};
    quint16 blockSize{DEFAULT_BLOCK_SIZE};
    QDateTime transferStartTime;
    TftpSessionState sessionState{TftpSessionState::Idle};
    bool isTransferring{false};

private:
    TftpClient *q_ptr{nullptr};
};

// TftpSession私有实现
class TftpSessionPrivate
{
public:
    explicit TftpSessionPrivate(TftpSession *q_ptr, const QHostAddress &clientAddress, quint16 clientPort);
    ~TftpSessionPrivate();

    void init();
    void resetSessionState();

public:
    QHostAddress clientAddress;
    quint16 clientPort{0};
    QString sessionId;
    QString rootPath;
    QString fileName;
    QFile *file{nullptr};
    qint64 fileSize{0};
    qint64 transferredBytes{0};
    quint16 blockIndex{1};
    quint16 blockSize{DEFAULT_BLOCK_SIZE};
    bool isUpload{false};
    TftpSessionState sessionState{TftpSessionState::Idle};
    QDateTime lastActivityTime;

private:
    TftpSession *q_ptr{nullptr};
};

// TftpServer私有实现
class TftpServerPrivate
{
public:
    explicit TftpServerPrivate(TftpServer *q_ptr);
    ~TftpServerPrivate();

    void init();

public:
    bool isRunning{false};
    quint16 port{DEFAULT_TFTP_PORT};
    QString rootPath;
    QHash<QString, TftpSession*> sessions;

private:
    TftpServer *q_ptr{nullptr};
};