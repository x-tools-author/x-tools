/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "tftpserver.h"
#include <QWidget>

namespace Ui {
class TftpServerUi;
}

class TftpServerUi : public QWidget
{
    Q_OBJECT

public:
    explicit TftpServerUi(QWidget *parent = nullptr);
    ~TftpServerUi();

private slots:
    // 浏览根目录
    void on_pushButtonBrowseRoot_clicked();

    // 启动服务器
    void on_pushButtonStart_clicked();

    // 停止服务器
    void on_pushButtonStop_clicked();

    // 处理客户端连接
    void handleClientConnected(const QHostAddress &clientAddress, quint16 clientPort);

    // 处理客户端断开
    void handleClientDisconnected(const QHostAddress &clientAddress, quint16 clientPort);

    // 处理传输进度
    void handleTransferProgress(const QString &fileName,
                                const QHostAddress &clientAddress,
                                int progress);

    // 处理错误
    void handleError(const QString &errorMessage);

    // 处理传输完成
    void handleTransferCompleted(const QString &fileName,
                                 const QHostAddress &clientAddress,
                                 bool isUpload);

private:
    // 初始化UI
    void initUI();

    // 记录日志
    void log(const QString &message);

    // 更新服务器状态
    void updateServerStatus(bool running);

    // 检查输入是否有效
    bool validateInput() const;

private:
    Ui::TftpServerUi *ui;
    TftpServer *m_tftpServer;
};
