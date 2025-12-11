/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftpclient.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>

TftpClient::TftpClient(QObject *parent)
    : QObject(parent)
    , m_tftp(new Tftp(this))
    , m_serverPort(69)
    , m_localWorkPath(QDir::currentPath())
    , m_isTransferring(false)
{
    connect(m_tftp, &Tftp::tftpProcessSignal, this, &TftpClient::onTftpProgress);
    connect(m_tftp, &Tftp::tftpErrorSignal, this, &TftpClient::onTftpError);
    connect(m_tftp, &Tftp::transferCompletedSignal, this, &TftpClient::onTransferCompleted);
}

TftpClient::~TftpClient()
{
    stop();
    delete m_tftp;
}

void TftpClient::setServerAddress(const QHostAddress &address, quint16 port)
{
    m_serverAddress = address;
    m_serverPort = port;
}

void TftpClient::setLocalWorkPath(const QString &path)
{
    m_localWorkPath = path;
    m_tftp->setWorkPathSlots(path);
}

bool TftpClient::downloadFile(const QString &remoteFileName, const QString &localFileName)
{
    if (m_isTransferring) {
        emit errorOccurred("Another transfer is already in progress");
        return false;
    }

    if (m_serverAddress.isNull()) {
        emit errorOccurred("Server address not set");
        return false;
    }

    // 确定本地文件名
    QString destFileName = localFileName;
    if (destFileName.isEmpty()) {
        QFileInfo fileInfo(remoteFileName);
        destFileName = fileInfo.fileName();
    }

    m_currentFileName = destFileName;
    m_isTransferring = true;

    // 设置客户端模式并配置服务器地址
    m_tftp->setClientMode(true);
    m_tftp->setDestinationAddress(m_serverAddress, m_serverPort);

    // 启动下载
    m_tftp->startDownload(remoteFileName, destFileName);

    return true;
}

bool TftpClient::uploadFile(const QString &localFileName, const QString &remoteFileName)
{
    if (m_isTransferring) {
        emit errorOccurred("Another transfer is already in progress");
        return false;
    }

    if (m_serverAddress.isNull()) {
        emit errorOccurred("Server address not set");
        return false;
    }

    // 检查本地文件是否存在
    QFileInfo fileInfo(m_localWorkPath + "/" + localFileName);
    if (!fileInfo.exists() || fileInfo.isDir()) {
        emit errorOccurred(QString("Local file not found: %1").arg(localFileName));
        return false;
    }

    // 确定远程文件名
    QString destFileName = remoteFileName;
    if (destFileName.isEmpty()) {
        destFileName = fileInfo.fileName();
    }

    m_currentFileName = destFileName;
    m_isTransferring = true;

    // 设置客户端模式并配置服务器地址
    m_tftp->setClientMode(true);
    m_tftp->setDestinationAddress(m_serverAddress, m_serverPort);

    // 启动上传
    m_tftp->startUpload(localFileName, destFileName);

    return true;
}

void TftpClient::stop()
{
    if (m_isTransferring) {
        m_tftp->stop();
        m_isTransferring = false;
        emit errorOccurred("Transfer stopped by user");
    }
}

void TftpClient::onTftpProgress(const QString &path, int value)
{
    emit progressChanged(m_currentFileName, value);

    if (value >= 100) {
        m_isTransferring = false;
        emit transferCompleted(m_currentFileName);
    }
}

void TftpClient::onTransferCompleted(const QString &fileName)
{
    Q_UNUSED(fileName)
    m_isTransferring = false;
    emit transferCompleted(m_currentFileName);
}
