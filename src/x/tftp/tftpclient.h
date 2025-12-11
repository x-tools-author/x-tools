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

class TftpClient : public QObject
{
    Q_OBJECT
public:
    explicit TftpClient(QObject *parent = nullptr);
    ~TftpClient();

    // 设置服务器地址和端口
    void setServerAddress(const QHostAddress &address, quint16 port = 69);
    
    // 设置本地工作目录
    void setLocalWorkPath(const QString &path);
    
    // 下载文件
    bool downloadFile(const QString &remoteFileName, const QString &localFileName = "");
    
    // 上传文件
    bool uploadFile(const QString &localFileName, const QString &remoteFileName = "");
    
    // 停止传输
    void stop();

signals:
    // 传输进度信号
    void progressChanged(const QString &fileName, int progress);
    
    // 错误信号
    void errorOccurred(const QString &errorMessage);
    
    // 传输完成信号
    void transferCompleted(const QString &fileName);

private slots:
    // 处理TFTP进度
    void onTftpProgress(const QString &path, int value);
    
    // 处理TFTP错误
    void onTftpError(const QString &errorMsg);

private:
    Tftp *m_tftp;
    QHostAddress m_serverAddress;
    quint16 m_serverPort;
    QString m_localWorkPath;
    QString m_currentFileName;
    bool m_isTransferring;
};
