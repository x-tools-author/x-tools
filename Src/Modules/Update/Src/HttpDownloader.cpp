/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 26, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181226  --v1.0.0 create the file.
 */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "HttpDownloader.h"
#include <QScopedPointer>
#include <QDebug>
#include <QDesktopServices>

HttpDownloader::HttpDownloader()
{
    moveToThread(this);
}

HttpDownloader::~HttpDownloader()
{

}

void HttpDownloader::run()
{
    exec();
}

void HttpDownloader::downloadFile(QUrl url)
{
    fileUrl = url;

    fileName = fileUrl.fileName();
    fileName.prepend(tempPath() + "/");

    if (QFile::exists(fileName)){
        QFile::remove(fileName);
    }

    file = openFileForWrite(fileName);
    if (!file){
        Q_ASSUME(0);
        return;
    }

    startRequest();
}

void HttpDownloader::startRequest()
{
    reply = qnam.get(QNetworkRequest(fileUrl));
    connect(reply, &QNetworkReply::finished, this, &HttpDownloader::downloadFinished);
#if 0
    /**
     * 在下载较大文件是，read()函数出现异常，所以，一直等到下载完成才读取所有数据（对于较大的资源，这样做并不好）
     */
    connect(reply, &QNetworkReply::readyRead, this, &HttpDownloader::read);
#endif
    connect(reply, &QNetworkReply::downloadProgress, this, &HttpDownloader::setProgress);
}

void HttpDownloader::cancelDownload()
{
    httpRequestAborted = true;

    if (reply){
        reply->abort();
    }
}

QString HttpDownloader::tempPath()
{
    QString str = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    return str;
}

QFile* HttpDownloader::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
        qWarning() << "open failed" << file->errorString();
        return nullptr;
    }
    return file.take();
}

void HttpDownloader::downloadFinished()
{
    QFileInfo fi;
    if (file) {
        read();
        fi.setFile(file->fileName());
        file->close();

        delete file;
        file = nullptr;
        emit completed();
    }

    if (httpRequestAborted) {
        reply->deleteLater();
        reply = nullptr;
        return;
    }

    if (reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        qWarning() <<  QString("Download failed:\n%1.").arg(reply->errorString());
        reply->deleteLater();
        reply = nullptr;
        return;
    }

    reply->deleteLater();
    reply = nullptr;

    QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteFilePath()));
}

void HttpDownloader::read()
{
    if (file){
        file->write(reply->readAll());
    }
}
