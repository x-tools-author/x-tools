/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftpserver.h"
#include <QDir>
#include <QDebug>

TftpServer::TftpServer(QObject *parent)
    : QObject(parent)
    , m_tftp(new Tftp(this))
    , m_isRunning(false)
    , m_port(0)
    , m_rootPath(QDir::currentPath())
{
    connect(m_tftp, &Tftp::tftpProcessSignal, this, &TftpServer::onTftpProgress);
    connect(m_tftp, &Tftp::tftpErrorSignal, this, &TftpServer::onTftpError);
}

TftpServer::~TftpServer()
{
    stop();
    delete m_tftp;
}

bool TftpServer::start(quint16 port)
{
    if (m_isRunning) {
        emit errorOccurred("Server is already running");
        return false;
    }

    m_port = port;
    m_tftp->setWorkPathSlots(m_rootPath);
    m_tftp->setPortSlots(port);
    m_tftp->start();
    
    m_isRunning = true;
    return true;
}

void TftpServer::stop()
{
    if (m_isRunning) {
        m_tftp->quit();
        m_tftp->wait();
        m_tftp->clearStatus();
        m_isRunning = false;
        m_port = 0;
    }
}

bool TftpServer::isRunning() const
{
    return m_isRunning;
}

void TftpServer::setRootPath(const QString &path)
{
    m_rootPath = path;
    if (m_isRunning) {
        m_tftp->setWorkPathSlots(path);
    }
}

quint16 TftpServer::port() const
{
    return m_port;
}

void TftpServer::onTftpProgress(const QString &path, int value)
{
    // 从路径中提取文件名
    QFileInfo fileInfo(path);
    QString fileName = fileInfo.fileName();
    
    // 注意：当前Tftp类没有提供客户端地址信息，这里使用默认地址
    QHostAddress clientAddress = QHostAddress::LocalHost;
    
    emit transferProgress(fileName, clientAddress, value);
    
    if (value >= 100) {
        // 假设是下载（服务器发送文件给客户端）
        emit transferCompleted(fileName, clientAddress, false);
    }
}

void TftpServer::onTftpError(const QString &errorMsg)
{
    emit errorOccurred(errorMsg);
}
