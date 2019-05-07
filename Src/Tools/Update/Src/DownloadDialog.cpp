/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
 */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "DownloadDialog.h"
#include "ui_DownloadDialog.h"
#include "HttpDownloader.h"

DownloadDialog::DownloadDialog(HttpDownloader* httpDownloader)
    :downloader(httpDownloader)
    ,ui(new Ui::DownloadDialog)
    ,enableUpdateSpeedLabel(false)
{
    ui->setupUi(this);
    progressBar = ui->progressBar;
    progressBar->setMinimum(0);
    internetSpeed = ui->labelSpeed;

    connect(downloader, &HttpDownloader::updateProgress, this, &DownloadDialog::updateProgress);
    connect(this, &DownloadDialog::rejected, downloader, &HttpDownloader::cancelDownload);
    connect(downloader, &HttpDownloader::completed, this, &DownloadDialog::close);

    speedTimer.setInterval(1000);
    connect(&speedTimer, &QTimer::timeout, this, &DownloadDialog::updateFlag);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
}

DownloadDialog::~DownloadDialog()
{

}

void DownloadDialog::downloadFile(QUrl url)
{
    internetSpeed->setText("0Bytes/s, 00:00:00:00");
    show();
    downloader->downloadFile(url);
    speedTimer.start();
}

void DownloadDialog::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    progressBar->setMaximum(static_cast<int>(bytesTotal));
    progressBar->setValue(static_cast<int>(bytesReceived));
    calculateInternetSpeed(bytesReceived, bytesTotal);
}

void DownloadDialog::calculateInternetSpeed(qint64 bytesReceived, qint64 bytesTotal)
{
    /// 该函数每秒更新一次
    if (!enableUpdateSpeedLabel){
        return;
    }

    static qint64 oldBytesReceived = bytesReceived;
    qint64 bytes = bytesReceived - oldBytesReceived;
    oldBytesReceived = bytesReceived;

    if (bytes > 0){
        /// 计算网速
        QString speed;
        if (bytes < 1024){
            speed = QString("%1 Bytes/s").arg(bytes);
        }else if (bytes < 1024*1024){
            speed = QString("%1 KB/s").arg(bytes/1024);
        }else {
            speed = QString("%1 M/s").arg(QString::number(static_cast<double>(bytes)/static_cast<double>(1024*1024), 'f', 2));
        }

        /// 计算剩余时间
        qint64 remainBytes = bytesTotal - bytesReceived;
        qint64 remainSeconds = remainBytes/bytes;

        qint64 days = 0;
        qint64 hours= 0;
        qint64 min  = 0;
        qint64 sed  = 0;

        days = remainSeconds/(60*60*24);
        hours= (remainSeconds%(60*60*24))/(60*60);
        min  = ((remainSeconds%(60*60*24))%(60*60))/60;
        sed  = remainSeconds%60;

        QString remainTime;
        remainTime = QString("%1:%2:%3:%4")
                .arg(QString::number(days), 2, '0')
                .arg(QString::number(hours), 2, '0')
                .arg(QString::number(min), 2, '0')
                .arg(QString::number(sed), 2, '0');

        internetSpeed->setText(speed + " " + remainTime);
    }

    enableUpdateSpeedLabel = false;
}

void DownloadDialog::updateFlag()
{
    enableUpdateSpeedLabel = true;
}
