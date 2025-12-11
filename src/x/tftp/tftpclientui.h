/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>
#include "tftpclient.h"

namespace Ui {
class TftpClientUi;
}

class TftpClientUi : public QWidget
{
    Q_OBJECT

public:
    explicit TftpClientUi(QWidget *parent = nullptr);
    ~TftpClientUi();

private slots:
    // 浏览本地路径
    void on_pushButtonBrowseLocal_clicked();
    
    // 下载文件
    void on_pushButtonDownload_clicked();
    
    // 上传文件
    void on_pushButtonUpload_clicked();
    
    // 停止传输
    void on_pushButtonStop_clicked();
    
    // 更新传输进度
    void updateProgress(int progress);
    
    // 处理错误
    void handleError(const QString &errorMessage);
    
    // 处理传输完成
    void handleTransferCompleted(const QString &fileName);

private:
    // 初始化UI
    void initUI();
    
    // 记录日志
    void log(const QString &message);
    
    // 检查输入是否有效
    bool validateInput() const;

private:
    Ui::TftpClientUi *ui;
    TftpClient *m_tftpClient;
};
