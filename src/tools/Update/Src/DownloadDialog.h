/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
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
