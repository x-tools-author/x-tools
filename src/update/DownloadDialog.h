/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>
#include <QUrl>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>

class HttpDownloader;

namespace Ui {
class DownloadDialog;
}

class DownloadDialog:public QDialog
{
    Q_OBJECT
public:
    DownloadDialog(HttpDownloader* httpDownloader);
    ~DownloadDialog();

    void downloadFile(QUrl url);
private:
    HttpDownloader* downloader;
    Ui::DownloadDialog* ui;
    QProgressBar* progressBar;
    QLabel* internetSpeed;
    QTimer speedTimer;
    bool enableUpdateSpeedLabel;
private slots:
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void calculateInternetSpeed(qint64 bytesReceived, qint64 bytesTotal);
    void updateFlag();
};

#endif
