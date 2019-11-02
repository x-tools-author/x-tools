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
#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H

#include <QThread>
#include <QUrl>
#include <QStandardPaths>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileInfo>

class HttpDownloader:public QThread
{
    Q_OBJECT
public:
    HttpDownloader();
    ~HttpDownloader();
public slots:
    void downloadFile(QUrl url);
    void cancelDownload();
protected:
    void run();
private:
    QUrl    fileUrl;
    QString fileName;
    QFile*  file;

    /// 该目录路径临时保存下载数据
    QString tempPath();
    QFile* openFileForWrite(const QString &fileName);

    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    bool httpRequestAborted;
private slots:
    void startRequest();
    void downloadFinished();
    void read();
    void setProgress(qint64 bytesReceived, qint64 bytesTotal){emit updateProgress(bytesReceived, bytesTotal);}
signals:
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void completed();
};

#endif
