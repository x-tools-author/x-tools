/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftp.h"
#include "tftp_p.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QStringList>
#include <QtEndian>

// TftpBasePrivate实现
TftpBasePrivate::TftpBasePrivate(TftpBase *q_ptr)
    : q_ptr(q_ptr)
{
    init();
}

TftpBasePrivate::~TftpBasePrivate()
{
    if (udpSocket) {
        udpSocket->close();
        delete udpSocket;
        udpSocket = nullptr;
    }

    if (timeoutTimer) {
        timeoutTimer->stop();
        delete timeoutTimer;
        timeoutTimer = nullptr;
    }
}

void TftpBasePrivate::init()
{
    udpSocket = new QUdpSocket;
    udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, MAX_BUFFER_SIZE);

    timeoutTimer = new QTimer;
    timeoutTimer->setInterval(TIMEOUT_INTERVAL);
    timeoutTimer->setSingleShot(true);

    retryCount = MAX_RETRY_COUNT;
}

// TftpBase实现
TftpBase::TftpBase(QObject *parent)
    : QObject(parent)
    , d_ptr(new TftpBasePrivate(this))
{
    X_D(TftpBase);

    connect(d->timeoutTimer, &QTimer::timeout, this, &TftpBase::handleTimeout);
    connect(d->udpSocket, &QUdpSocket::readyRead, this, &TftpBase::processPendingDatagrams);
}

TftpBase::~TftpBase()
{
    X_D(TftpBase);
    delete d;
}

QUdpSocket *TftpBase::udpSocket() const
{
    X_D(const TftpBase);
    return d->udpSocket;
}

void TftpBase::sendDatagram(const QByteArray &data, const QHostAddress &address, quint16 port)
{
    X_D(TftpBase);

    // 保存最后发送的数据和地址
    d->lastSentData = data;
    d->lastSentAddress = address;
    d->lastSentPort = port;

    // 发送数据
    d->udpSocket->writeDatagram(data, address, port);

    // 启动超时定时器
    d->retryCount = MAX_RETRY_COUNT;
    d->timeoutTimer->start();
}

void TftpBase::sendErrorCode(TftpErrorCode code,
                             const QString &message,
                             const QHostAddress &address,
                             quint16 port)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // 写入操作码
    quint16 opcode = ERROR;
    stream << opcode;

    // 写入错误码
    quint16 errorCode = static_cast<quint16>(code);
    stream << errorCode;

    // 写入错误消息
    data.append(message.toUtf8());
    data.append('\0');

    // 发送错误消息
    sendDatagram(data, address, port);
}

QMap<QString, QString> TftpBase::parseTftpOptions(const char *data, quint32 dataLength)
{
    QMap<QString, QString> options;

    const char *ptr = data;
    const char *end = data + dataLength;

    // 跳过文件名和模式
    while (ptr < end && *ptr != '\0') {
        ptr++;
    }
    if (ptr < end) {
        ptr++;
    }

    while (ptr < end && *ptr != '\0') {
        ptr++;
    }
    if (ptr < end) {
        ptr++;
    }

    // 解析选项
    while (ptr < end) {
        const char *keyStart = ptr;
        while (ptr < end && *ptr != '\0') {
            ptr++;
        }
        if (ptr >= end) {
            break;
        }
        QString key(keyStart, ptr - keyStart);
        ptr++;

        const char *valueStart = ptr;
        while (ptr < end && *ptr != '\0') {
            ptr++;
        }
        if (ptr >= end) {
            break;
        }
        QString value(valueStart, ptr - valueStart);
        ptr++;

        options.insert(key, value);
    }

    return options;
}

QByteArray TftpBase::createTftpOptions(const QMap<QString, QString> &options)
{
    QByteArray result;

    for (auto it = options.constBegin(); it != options.constEnd(); ++it) {
        result.append(it.key().toUtf8());
        result.append('\0');
        result.append(it.value().toUtf8());
        result.append('\0');
    }

    return result;
}

QString TftpBase::calculateFileMd5(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    if (!hash.addData(&file)) {
        return QString();
    }

    return hash.result().toHex();
}

QString TftpBase::generateSessionId()
{
    QDateTime now = QDateTime::currentDateTime();
    qint64 timestamp = now.toMSecsSinceEpoch();
    quint32 random = QRandomGenerator::global()->generate();

    QString sessionId = QString("%1-%2").arg(timestamp).arg(random, 8, 16, QChar('0'));
    return sessionId;
}

// TftpClientPrivate实现
TftpClientPrivate::TftpClientPrivate(TftpClient *q_ptr)
    : q_ptr(q_ptr)
{
    init();
}

TftpClientPrivate::~TftpClientPrivate()
{
    resetTransferState();
}

void TftpClientPrivate::init()
{
    serverPort = DEFAULT_TFTP_PORT;
    localWorkPath = QDir::currentPath();
    resetTransferState();
}

void TftpClientPrivate::resetTransferState()
{
    if (file) {
        file->close();
        delete file;
        file = nullptr;
    }

    currentFileName.clear();
    fileSize = 0;
    transferredBytes = 0;
    blockIndex = 1;
    blockSize = DEFAULT_BLOCK_SIZE;
    sessionState = TftpSessionState::Idle;
    isTransferring = false;
}

// TftpClient实现
TftpClient::TftpClient(QObject *parent)
    : TftpBase(parent)
    , d_ptr(new TftpClientPrivate(this))
{}

TftpClient::~TftpClient()
{
    X_D(TftpClient);
    delete d;
}

void TftpClient::setServerAddress(const QHostAddress &address, quint16 port)
{
    X_D(TftpClient);
    d->serverAddress = address;
    d->serverPort = port;
}

void TftpClient::setLocalWorkPath(const QString &path)
{
    X_D(TftpClient);
    d->localWorkPath = path;
}

bool TftpClient::downloadFile(const QString &remoteFileName, const QString &localFileName)
{
    X_D(TftpClient);

    if (d->isTransferring) {
        emit errorOccurred(tr("Another transfer is already in progress"));
        return false;
    }

    // 重置传输状态
    d->resetTransferState();

    // 确定本地文件名
    QString localFile = localFileName;
    if (localFile.isEmpty()) {
        QFileInfo info(remoteFileName);
        localFile = info.fileName();
    }

    // 构建完整的本地文件路径
    QString filePath = QDir(d->localWorkPath).filePath(localFile);

    // 检查文件是否已存在
    if (QFile::exists(filePath)) {
        emit errorOccurred(tr("File already exists: %1").arg(filePath));
        return false;
    }

    // 创建文件
    d->file = new QFile(filePath);
    if (!d->file->open(QIODevice::WriteOnly)) {
        emit errorOccurred(tr("Failed to open file for writing: %1").arg(filePath));
        delete d->file;
        d->file = nullptr;
        return false;
    }

    // 设置传输状态
    d->currentFileName = localFile;
    d->sessionState = TftpSessionState::ReadRequestSent;
    d->isTransferring = true;
    d->transferStartTime = QDateTime::currentDateTime();

    // 创建RRQ请求
    QByteArray request;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // 写入操作码
    quint16 opcode = RRQ;
    stream << opcode;

    // 写入文件名
    request.append(remoteFileName.toUtf8());
    request.append('\0');

    // 写入模式
    request.append(TFTP_MODE_OCTET);
    request.append('\0');

    // 写入选项
    QMap<QString, QString> options;
    options.insert(TFTP_OPTION_BLOCK_SIZE, QString::number(DEFAULT_BLOCK_SIZE));
    options.insert(TFTP_OPTION_TIMEOUT, QString::number(TIMEOUT_INTERVAL / 1000));
    request.append(createTftpOptions(options));

    // 发送请求
    sendDatagram(request, d->serverAddress, d->serverPort);

    return true;
}

bool TftpClient::uploadFile(const QString &localFileName, const QString &remoteFileName)
{
    X_D(TftpClient);

    if (d->isTransferring) {
        emit errorOccurred(tr("Another transfer is already in progress"));
        return false;
    }

    // 重置传输状态
    d->resetTransferState();

    // 构建完整的本地文件路径
    QString filePath = QDir(d->localWorkPath).filePath(localFileName);

    // 检查文件是否存在
    if (!QFile::exists(filePath)) {
        emit errorOccurred(tr("File not found: %1").arg(filePath));
        return false;
    }

    // 打开文件
    d->file = new QFile(filePath);
    if (!d->file->open(QIODevice::ReadOnly)) {
        emit errorOccurred(tr("Failed to open file for reading: %1").arg(filePath));
        delete d->file;
        d->file = nullptr;
        return false;
    }

    // 设置传输状态
    d->fileSize = d->file->size();
    d->currentFileName = remoteFileName.isEmpty() ? QFileInfo(filePath).fileName() : remoteFileName;
    d->sessionState = TftpSessionState::WriteRequestSent;
    d->isTransferring = true;
    d->transferStartTime = QDateTime::currentDateTime();

    // 创建WRQ请求
    QByteArray request;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // 写入操作码
    quint16 opcode = WRQ;
    stream << opcode;

    // 写入文件名
    request.append(d->currentFileName.toUtf8());
    request.append('\0');

    // 写入模式
    request.append(TFTP_MODE_OCTET);
    request.append('\0');

    // 写入选项
    QMap<QString, QString> options;
    options.insert(TFTP_OPTION_BLOCK_SIZE, QString::number(DEFAULT_BLOCK_SIZE));
    options.insert(TFTP_OPTION_TIMEOUT, QString::number(TIMEOUT_INTERVAL / 1000));
    options.insert(TFTP_OPTION_TRANSFER_SIZE, QString::number(d->fileSize));
    request.append(createTftpOptions(options));

    // 发送请求
    sendDatagram(request, d->serverAddress, d->serverPort);

    return true;
}

void TftpClient::stop()
{
    X_D(TftpClient);

    if (!d->isTransferring) {
        return;
    }

    // 重置传输状态
    d->resetTransferState();
}

bool TftpClient::isTransferring() const
{
    X_D(const TftpClient);
    return d->isTransferring;
}

void TftpClient::processPendingDatagrams()
{
    X_D(TftpClient);

    while (udpSocket()->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress senderAddress;
        quint16 senderPort;

        datagram.resize(udpSocket()->pendingDatagramSize());
        udpSocket()->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        // 检查是否是来自服务器的数据包
        if (senderAddress != d->serverAddress) {
            continue;
        }

        // 解析数据包
        QDataStream stream(&datagram, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::BigEndian);

        quint16 opcode;
        stream >> opcode;

        switch (opcode) {
        case DATA: {
            // 处理DATA数据包
            if (d->sessionState != TftpSessionState::Transferring) {
                d->sessionState = TftpSessionState::Transferring;
            }

            quint16 blockNumber;
            stream >> blockNumber;

            // 检查块号是否正确
            if (blockNumber != d->blockIndex) {
                // 块号不正确，忽略此数据包
                continue;
            }

            // 读取数据
            QByteArray data = datagram.mid(4);
            qint64 bytesWritten = d->file->write(data);

            if (bytesWritten != data.size()) {
                emit errorOccurred(tr("Failed to write data to file"));
                stop();
                return;
            }

            // 更新传输状态
            d->transferredBytes += bytesWritten;
            d->blockIndex++;

            // 发送ACK
            QByteArray ackData;
            QDataStream ackStream(&ackData, QIODevice::WriteOnly);
            ackStream.setByteOrder(QDataStream::BigEndian);
            ackStream << static_cast<quint16>(ACK);
            ackStream << blockNumber;

            sendDatagram(ackData, senderAddress, senderPort);

            // 计算进度
            int speed = 0;
            if (!d->transferStartTime.isNull()) {
                qint64 elapsed = d->transferStartTime.msecsTo(QDateTime::currentDateTime());
                if (elapsed > 0) {
                    speed = static_cast<int>((d->transferredBytes * 1000.0) / elapsed);
                }
            }

            // 发送进度信号
            emit progressChanged(d->currentFileName, d->transferredBytes, d->fileSize, speed);

            // 检查是否传输完成
            if (data.size() < d->blockSize) {
                // 传输完成
                d->file->close();
                delete d->file;
                d->file = nullptr;

                d->sessionState = TftpSessionState::Completed;
                d->isTransferring = false;

                emit transferCompleted(d->currentFileName);
            }

            break;
        }
        case ACK: {
            // 处理ACK数据包
            quint16 blockNumber;
            stream >> blockNumber;

            // 检查是否是对写入请求的ACK
            if (d->sessionState == TftpSessionState::WriteRequestSent) {
                d->sessionState = TftpSessionState::Transferring;
            }

            // 检查块号
            if (blockNumber != d->blockIndex - 1) {
                // 块号不正确，忽略此数据包
                continue;
            }

            // 读取下一个数据块
            QByteArray data = d->file->read(d->blockSize);

            if (data.isEmpty()) {
                // 文件读取完成
                if (d->file->atEnd()) {
                    // 发送最后一个数据包
                    if (blockNumber == 0) {
                        // 如果文件为空，发送一个空的数据块
                        QByteArray emptyData;
                        QDataStream emptyStream(&emptyData, QIODevice::WriteOnly);
                        emptyStream.setByteOrder(QDataStream::BigEndian);
                        emptyStream << static_cast<quint16>(DATA);
                        emptyStream << static_cast<quint16>(d->blockIndex);

                        sendDatagram(emptyData, senderAddress, senderPort);
                    } else {
                        // 传输完成
                        d->file->close();
                        delete d->file;
                        d->file = nullptr;

                        d->sessionState = TftpSessionState::Completed;
                        d->isTransferring = false;

                        emit transferCompleted(d->currentFileName);
                    }
                } else {
                    // 读取错误
                    emit errorOccurred(tr("Failed to read data from file"));
                    stop();
                }
                return;
            }

            // 创建DATA数据包
            QByteArray dataPacket;
            QDataStream dataStream(&dataPacket, QIODevice::WriteOnly);
            dataStream.setByteOrder(QDataStream::BigEndian);
            dataStream << static_cast<quint16>(DATA);
            dataStream << static_cast<quint16>(d->blockIndex);
            dataStream.writeRawData(data.data(), data.size());

            // 发送DATA数据包
            sendDatagram(dataPacket, senderAddress, senderPort);

            // 更新传输状态
            d->transferredBytes += data.size();
            d->blockIndex++;

            // 计算进度
            int speed = 0;
            if (!d->transferStartTime.isNull()) {
                qint64 elapsed = d->transferStartTime.msecsTo(QDateTime::currentDateTime());
                if (elapsed > 0) {
                    speed = static_cast<int>((d->transferredBytes * 1000.0) / elapsed);
                }
            }

            // 发送进度信号
            emit progressChanged(d->currentFileName, d->transferredBytes, d->fileSize, speed);

            break;
        }
        case OACK: {
            // 处理OACK数据包
            QMap<QString, QString> options = parseTftpOptions(datagram.constData(), datagram.size());

            // 解析块大小选项
            if (options.contains(TFTP_OPTION_BLOCK_SIZE)) {
                bool ok;
                quint16 blksize = options.value(TFTP_OPTION_BLOCK_SIZE).toUShort(&ok);
                if (ok && blksize > 0 && blksize <= 65464) {
                    d->blockSize = blksize;
                }
            }

            // 解析文件大小选项（仅用于下载）
            if (options.contains(TFTP_OPTION_TRANSFER_SIZE)) {
                bool ok;
                qint64 tsize = options.value(TFTP_OPTION_TRANSFER_SIZE).toLongLong(&ok);
                if (ok) {
                    d->fileSize = tsize;
                }
            }

            // 根据当前状态处理
            if (d->sessionState == TftpSessionState::ReadRequestSent) {
                // 下载请求，发送第一个ACK
                QByteArray ackData;
                QDataStream ackStream(&ackData, QIODevice::WriteOnly);
                ackStream.setByteOrder(QDataStream::BigEndian);
                ackStream << static_cast<quint16>(ACK);
                ackStream << static_cast<quint16>(0);

                sendDatagram(ackData, senderAddress, senderPort);

                d->sessionState = TftpSessionState::Transferring;
            } else if (d->sessionState == TftpSessionState::WriteRequestSent) {
                // 上传请求，开始发送数据
                d->sessionState = TftpSessionState::Transferring;

                // 发送第一个数据块
                QByteArray data = d->file->read(d->blockSize);

                if (data.isEmpty() && !d->file->atEnd()) {
                    // 读取错误
                    emit errorOccurred(tr("Failed to read data from file"));
                    stop();
                    return;
                }

                QByteArray dataPacket;
                QDataStream dataStream(&dataPacket, QIODevice::WriteOnly);
                dataStream.setByteOrder(QDataStream::BigEndian);
                dataStream << static_cast<quint16>(DATA);
                dataStream << static_cast<quint16>(d->blockIndex);
                dataStream.writeRawData(data.data(), data.size());

                sendDatagram(dataPacket, senderAddress, senderPort);

                d->transferredBytes += data.size();
                d->blockIndex++;
            }

            break;
        }
        case ERROR: {
            // 处理ERROR数据包
            quint16 errorCode;
            stream >> errorCode;

            // 读取错误消息
            QString errorMessage = datagram.mid(4);
            errorMessage.chop(1); // 移除末尾的\0

            emit errorOccurred(errorMessage);
            stop();

            break;
        }
        default:
            // 未知操作码
            emit errorOccurred(tr("Unknown TFTP opcode received"));
            stop();
            break;
        }
    }
}

void TftpClient::handleTimeout()
{
    X_D(TftpClient);

    if (d->retryCount <= 0) {
        // 重试次数已用完
        emit errorOccurred(tr("Connection timeout"));
        stop();
        return;
    }

    // 重新发送最后一个数据包
    d->retryCount--;
    sendDatagram(d->lastSentData, d->lastSentAddress, d->lastSentPort);
}

// TftpSessionPrivate实现
TftpSessionPrivate::TftpSessionPrivate(TftpSession *q_ptr,
                                       const QHostAddress &clientAddress,
                                       quint16 clientPort)
    : q_ptr(q_ptr)
    , clientAddress(clientAddress)
    , clientPort(clientPort)
{
    init();
}

TftpSessionPrivate::~TftpSessionPrivate()
{
    resetSessionState();
}

void TftpSessionPrivate::init()
{
    sessionId = generateSessionId();
    rootPath = QDir::currentPath();
    resetSessionState();
}

void TftpSessionPrivate::resetSessionState()
{
    if (file) {
        file->close();
        delete file;
        file = nullptr;
    }

    fileName.clear();
    fileSize = 0;
    transferredBytes = 0;
    blockIndex = 1;
    blockSize = DEFAULT_BLOCK_SIZE;
    isUpload = false;
    sessionState = TftpSessionState::Idle;
    lastActivityTime = QDateTime::currentDateTime();
}

// TftpSession实现
TftpSession::TftpSession(const QHostAddress &clientAddress, quint16 clientPort, QObject *parent)
    : TftpBase(parent)
    , d_ptr(new TftpSessionPrivate(this, clientAddress, clientPort))
{}

TftpSession::~TftpSession()
{
    X_D(TftpSession);
    delete d;
}

QHostAddress TftpSession::clientAddress() const
{
    X_D(const TftpSession);
    return d->clientAddress;
}

quint16 TftpSession::clientPort() const
{
    X_D(const TftpSession);
    return d->clientPort;
}

QString TftpSession::sessionId() const
{
    X_D(const TftpSession);
    return d->sessionId;
}

void TftpSession::handleRequest(const QByteArray &data)
{
    X_D(TftpSession);

    d->lastActivityTime = QDateTime::currentDateTime();

    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    quint16 opcode;
    stream >> opcode;

    switch (opcode) {
    case RRQ: {
        // 处理读取请求
        d->resetSessionState();
        d->isUpload = false;

        // 解析文件名
        const char *nameStart = data.constData() + 2;
        const char *nameEnd = qstrnlen(nameStart, data.size() - 2) + nameStart;
        d->fileName = QString::fromUtf8(nameStart, nameEnd - nameStart);

        // 构建完整的文件路径
        QString filePath = QDir(d->rootPath).filePath(d->fileName);

        // 检查文件是否存在
        if (!QFile::exists(filePath)) {
            sendErrorCode(FILE_NOT_FOUND, tr("File not found"), d->clientAddress, d->clientPort);
            return;
        }

        // 打开文件
        d->file = new QFile(filePath);
        if (!d->file->open(QIODevice::ReadOnly)) {
            sendErrorCode(ACCESS_VIOLATION, tr("Access violation"), d->clientAddress, d->clientPort);
            return;
        }

        // 获取文件大小
        d->fileSize = d->file->size();

        // 解析选项
        QMap<QString, QString> options = parseTftpOptions(data.constData(), data.size());

        // 处理块大小选项
        if (options.contains(TFTP_OPTION_BLOCK_SIZE)) {
            bool ok;
            quint16 blksize = options.value(TFTP_OPTION_BLOCK_SIZE).toUShort(&ok);
            if (ok && blksize > 0 && blksize <= 65464) {
                d->blockSize = blksize;
            }
        }

        // 发送OACK
        QMap<QString, QString> oackOptions;
        oackOptions.insert(TFTP_OPTION_BLOCK_SIZE, QString::number(d->blockSize));
        oackOptions.insert(TFTP_OPTION_TRANSFER_SIZE, QString::number(d->fileSize));

        QByteArray oackData;
        QDataStream oackStream(&oackData, QIODevice::WriteOnly);
        oackStream.setByteOrder(QDataStream::BigEndian);
        oackStream << static_cast<quint16>(OACK);
        oackData.append(createTftpOptions(oackOptions));

        sendDatagram(oackData, d->clientAddress, d->clientPort);

        d->sessionState = TftpSessionState::Transferring;

        break;
    }
    case WRQ: {
        // 处理写入请求
        d->resetSessionState();
        d->isUpload = true;

        // 解析文件名
        const char *nameStart = data.constData() + 2;
        const char *nameEnd = qstrnlen(nameStart, data.size() - 2) + nameStart;
        d->fileName = QString::fromUtf8(nameStart, nameEnd - nameStart);

        // 构建完整的文件路径
        QString filePath = QDir(d->rootPath).filePath(d->fileName);

        // 检查文件是否已存在
        if (QFile::exists(filePath)) {
            sendErrorCode(FILE_ALREADY_EXISTS,
                          tr("File already exists"),
                          d->clientAddress,
                          d->clientPort);
            return;
        }

        // 打开文件
        d->file = new QFile(filePath);
        if (!d->file->open(QIODevice::WriteOnly)) {
            sendErrorCode(ACCESS_VIOLATION, tr("Access violation"), d->clientAddress, d->clientPort);
            return;
        }

        // 解析选项
        QMap<QString, QString> options = parseTftpOptions(data.constData(), data.size());

        // 处理块大小选项
        if (options.contains(TFTP_OPTION_BLOCK_SIZE)) {
            bool ok;
            quint16 blksize = options.value(TFTP_OPTION_BLOCK_SIZE).toUShort(&ok);
            if (ok && blksize > 0 && blksize <= 65464) {
                d->blockSize = blksize;
            }
        }

        // 处理文件大小选项
        if (options.contains(TFTP_OPTION_TRANSFER_SIZE)) {
            bool ok;
            qint64 tsize = options.value(TFTP_OPTION_TRANSFER_SIZE).toLongLong(&ok);
            if (ok) {
                d->fileSize = tsize;
            }
        }

        // 发送OACK
        QMap<QString, QString> oackOptions;
        oackOptions.insert(TFTP_OPTION_BLOCK_SIZE, QString::number(d->blockSize));

        QByteArray oackData;
        QDataStream oackStream(&oackData, QIODevice::WriteOnly);
        oackStream.setByteOrder(QDataStream::BigEndian);
        oackStream << static_cast<quint16>(OACK);
        oackData.append(createTftpOptions(oackOptions));

        sendDatagram(oackData, d->clientAddress, d->clientPort);

        d->sessionState = TftpSessionState::Transferring;

        break;
    }
    default:
        // 未知操作码
        sendErrorCode(ILLEGAL_OPERATION,
                      tr("Illegal TFTP operation"),
                      d->clientAddress,
                      d->clientPort);
        break;
    }
}

bool TftpSession::isActive() const
{
    X_D(const TftpSession);
    return d->sessionState != TftpSessionState::Idle
           && d->sessionState != TftpSessionState::Completed
           && d->sessionState != TftpSessionState::Error;
}

void TftpSession::setRootPath(const QString &path)
{
    X_D(TftpSession);
    d->rootPath = path;
}

void TftpSession::processPendingDatagrams()
{
    X_D(TftpSession);

    while (udpSocket()->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress senderAddress;
        quint16 senderPort;

        datagram.resize(udpSocket()->pendingDatagramSize());
        udpSocket()->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        // 检查是否是来自正确客户端的数据包
        if (senderAddress != d->clientAddress || senderPort != d->clientPort) {
            continue;
        }

        d->lastActivityTime = QDateTime::currentDateTime();

        QDataStream stream(&datagram, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::BigEndian);

        quint16 opcode;
        stream >> opcode;

        switch (opcode) {
        case ACK: {
            // 处理ACK数据包（仅用于下载）
            if (d->isUpload) {
                // 上传时不应该收到ACK
                sendErrorCode(ILLEGAL_OPERATION,
                              tr("Illegal TFTP operation"),
                              senderAddress,
                              senderPort);
                return;
            }

            quint16 blockNumber;
            stream >> blockNumber;

            // 检查块号是否正确
            if (blockNumber + 1 != d->blockIndex) {
                // 块号不正确，忽略此数据包
                continue;
            }

            // 读取下一个数据块
            QByteArray data = d->file->read(d->blockSize);

            if (data.isEmpty() && !d->file->atEnd()) {
                // 读取错误
                sendErrorCode(ACCESS_VIOLATION, tr("Access violation"), senderAddress, senderPort);
                return;
            }

            // 发送DATA数据包
            QByteArray dataPacket;
            QDataStream dataStream(&dataPacket, QIODevice::WriteOnly);
            dataStream.setByteOrder(QDataStream::BigEndian);
            dataStream << static_cast<quint16>(DATA);
            dataStream << static_cast<quint16>(d->blockIndex);
            dataStream.writeRawData(data.data(), data.size());

            sendDatagram(dataPacket, senderAddress, senderPort);

            // 更新传输状态
            d->transferredBytes += data.size();
            d->blockIndex++;

            // 发送进度信号
            emit progressChanged(d->fileName, d->transferredBytes, d->fileSize);

            // 检查是否传输完成
            if (data.size() < d->blockSize) {
                // 传输完成
                d->file->close();
                delete d->file;
                d->file = nullptr;

                d->sessionState = TftpSessionState::Completed;

                emit transferCompleted(d->fileName, false);
                emit sessionFinished(d->sessionId);
            }

            break;
        }
        case DATA: {
            // 处理DATA数据包（仅用于上传）
            if (!d->isUpload) {
                // 下载时不应该收到DATA
                sendErrorCode(ILLEGAL_OPERATION,
                              tr("Illegal TFTP operation"),
                              senderAddress,
                              senderPort);
                return;
            }

            quint16 blockNumber;
            stream >> blockNumber;

            // 检查块号是否正确
            if (blockNumber != d->blockIndex) {
                // 块号不正确，忽略此数据包
                continue;
            }

            // 读取数据
            QByteArray data = datagram.mid(4);
            qint64 bytesWritten = d->file->write(data);

            if (bytesWritten != data.size()) {
                sendErrorCode(DISK_FULL,
                              tr("Disk full or allocation exceeded"),
                              senderAddress,
                              senderPort);
                return;
            }

            // 发送ACK
            QByteArray ackData;
            QDataStream ackStream(&ackData, QIODevice::WriteOnly);
            ackStream.setByteOrder(QDataStream::BigEndian);
            ackStream << static_cast<quint16>(ACK);
            ackStream << blockNumber;

            sendDatagram(ackData, senderAddress, senderPort);

            // 更新传输状态
            d->transferredBytes += bytesWritten;
            d->blockIndex++;

            // 发送进度信号
            emit progressChanged(d->fileName, d->transferredBytes, d->fileSize);

            // 检查是否传输完成
            if (data.size() < d->blockSize) {
                // 传输完成
                d->file->close();
                delete d->file;
                d->file = nullptr;

                d->sessionState = TftpSessionState::Completed;

                emit transferCompleted(d->fileName, true);
                emit sessionFinished(d->sessionId);
            }

            break;
        }
        case ERROR: {
            // 处理错误数据包
            d->sessionState = TftpSessionState::Error;

            // 发送会话结束信号
            emit sessionFinished(d->sessionId);

            break;
        }
        default:
            // 未知操作码
            sendErrorCode(ILLEGAL_OPERATION,
                          tr("Illegal TFTP operation"),
                          senderAddress,
                          senderPort);
            break;
        }
    }
}

void TftpSession::handleTimeout()
{
    X_D(TftpSession);

    if (d->retryCount <= 0) {
        // 重试次数已用完
        d->sessionState = TftpSessionState::Error;
        emit errorOccurred(tr("Connection timeout"));
        emit sessionFinished(d->sessionId);
        return;
    }

    // 重新发送最后一个数据包
    d->retryCount--;
    sendDatagram(d->lastSentData, d->lastSentAddress, d->lastSentPort);
}

// TftpServerPrivate实现
TftpServerPrivate::TftpServerPrivate(TftpServer *q_ptr)
    : q_ptr(q_ptr)
{
    init();
}

TftpServerPrivate::~TftpServerPrivate()
{
    // 清理所有会话
    qDeleteAll(sessions);
    sessions.clear();
}

void TftpServerPrivate::init()
{
    port = DEFAULT_TFTP_PORT;
    rootPath = QDir::currentPath();
    isRunning = false;
}

// TftpServer实现
TftpServer::TftpServer(QObject *parent)
    : TftpBase(parent)
    , d_ptr(new TftpServerPrivate(this))
{}

TftpServer::~TftpServer()
{
    X_D(TftpServer);
    delete d;
}

bool TftpServer::start(quint16 port)
{
    X_D(TftpServer);

    if (d->isRunning) {
        // 服务器已经在运行
        return false;
    }

    // 绑定UDP端口
    if (!udpSocket()->bind(QHostAddress::Any, port)) {
        emit errorOccurred(tr("Failed to bind to port %1").arg(port));
        return false;
    }

    d->port = port;
    d->isRunning = true;

    return true;
}

void TftpServer::stop()
{
    X_D(TftpServer);

    if (!d->isRunning) {
        return;
    }

    // 关闭UDP socket
    udpSocket()->close();

    // 清理所有会话
    qDeleteAll(d->sessions);
    d->sessions.clear();

    d->isRunning = false;
}

bool TftpServer::isRunning() const
{
    X_D(const TftpServer);
    return d->isRunning;
}

void TftpServer::setRootPath(const QString &path)
{
    X_D(TftpServer);
    d->rootPath = path;

    // 更新所有现有会话的根路径
    for (TftpSession *session : d->sessions) {
        session->setRootPath(path);
    }
}

quint16 TftpServer::port() const
{
    X_D(const TftpServer);
    return d->port;
}

void TftpServer::processPendingDatagrams()
{
    X_D(TftpServer);

    while (udpSocket()->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress senderAddress;
        quint16 senderPort;

        datagram.resize(udpSocket()->pendingDatagramSize());
        udpSocket()->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        QDataStream stream(&datagram, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::BigEndian);

        quint16 opcode;
        stream >> opcode;

        // 检查是否是RRQ或WRQ请求
        if (opcode == RRQ || opcode == WRQ) {
            // 查找是否已有该客户端的会话
            QString key = QString("%1:%2").arg(senderAddress.toString()).arg(senderPort);
            TftpSession *session = d->sessions.value(key);

            if (!session) {
                // 创建新会话
                session = new TftpSession(senderAddress, senderPort, this);
                session->setRootPath(d->rootPath);

                // 连接会话信号
                connect(session,
                        &TftpSession::progressChanged,
                        this,
                        [this,
                         senderAddress](const QString &fileName, qint64 transferred, qint64 total) {
                            emit transferProgress(fileName, senderAddress, transferred, total);
                        });

                connect(session,
                        &TftpSession::transferCompleted,
                        this,
                        [this](const QString &fileName,
                               const QHostAddress &clientAddress,
                               bool isUpload) {
                            emit transferCompleted(fileName, clientAddress, isUpload);
                        });

                connect(session,
                        &TftpSession::sessionFinished,
                        this,
                        &TftpServer::onSessionFinished);

                connect(session, &TftpSession::errorOccurred, this, &TftpServer::errorOccurred);

                d->sessions.insert(key, session);

                // 发送客户端连接信号
                emit clientConnected(senderAddress, senderPort);
            }

            // 处理请求
            session->handleRequest(datagram);
        } else {
            // 其他数据包，查找对应的会话
            QString key = QString("%1:%2").arg(senderAddress.toString()).arg(senderPort);
            TftpSession *session = d->sessions.value(key);

            if (session) {
                // 将会话的socket替换为服务器的socket
                // 注意：这是一个简化的实现，实际应该使用会话自己的socket
                // 这里为了简化，直接让会话处理数据包
                QMetaObject::invokeMethod(session, "processPendingDatagrams");
            } else {
                // 没有找到对应的会话
                sendErrorCode(UNKNOWN_TRANSFER_ID,
                              tr("Unknown transfer ID"),
                              senderAddress,
                              senderPort);
            }
        }
    }
}

void TftpServer::handleTimeout()
{
    // 服务器不需要处理超时，超时由各个会话处理
}

void TftpServer::onSessionFinished(const QString &sessionId)
{
    X_D(TftpServer);

    // 查找并移除会话
    for (auto it = d->sessions.begin(); it != d->sessions.end(); ++it) {
        TftpSession *session = it.value();
        if (session->sessionId() == sessionId) {
            // 发送客户端断开连接信号
            emit clientDisconnected(session->clientAddress(), session->clientPort());

            // 移除会话
            delete session;
            d->sessions.erase(it);
            break;
        }
    }
}
