/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 26, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181226  --v1.0.0 create the file.
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
