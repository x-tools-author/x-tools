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
#include <QObject>
#include <QString>
#include <QHostAddress>

class TftpServer : public QObject
{
    Q_OBJECT
public:
    explicit TftpServer(QObject *parent = nullptr);
    ~TftpServer();

    // 启动服务器
    bool start(quint16 port = 69);
    
    // 停止服务器
    void stop();
    
    // 检查服务器是否正在运行
    bool isRunning() const;
    
    // 设置服务器根目录
    void setRootPath(const QString &path);
    
    // 获取当前端口
    quint16 port() const;

signals:
    // 客户端连接信号
    void clientConnected(const QHostAddress &clientAddress, quint16 clientPort);
    
    // 客户端断开连接信号
    void clientDisconnected(const QHostAddress &clientAddress, quint16 clientPort);
    
    // 文件传输进度信号
    void transferProgress(const QString &fileName, const QHostAddress &clientAddress, int progress);
    
    // 错误信号
    void errorOccurred(const QString &errorMessage);
    
    // 文件传输完成信号
    void transferCompleted(const QString &fileName, const QHostAddress &clientAddress, bool isUpload);

private slots:
    // 处理TFTP进度
    void onTftpProgress(const QString &path, int value);
    
    // 处理TFTP错误
    void onTftpError(const QString &errorMsg);

private:
    Tftp *m_tftp;
    bool m_isRunning;
    quint16 m_port;
    QString m_rootPath;
};
