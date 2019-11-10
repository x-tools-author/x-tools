/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QScopedPointer>
#include <QDesktopServices>

#include "SAKUpdateDownloader.hh"

SAKUpdateDownloader::SAKUpdateDownloader()
{
    moveToThread(this);
}

SAKUpdateDownloader::~SAKUpdateDownloader()
{

}

void SAKUpdateDownloader::run()
{
    exec();
}

void SAKUpdateDownloader::downloadFile(QUrl url)
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

void SAKUpdateDownloader::startRequest()
{
    reply = qnam.get(QNetworkRequest(fileUrl));
    connect(reply, &QNetworkReply::finished, this, &SAKUpdateDownloader::downloadFinished);
#if 0
    /**
     * 在下载较大文件是，read()函数出现异常，所以，一直等到下载完成才读取所有数据（对于较大的资源，这样做并不好）
     */
    connect(reply, &QNetworkReply::readyRead, this, &SAKUpdateDownloader::read);
#endif
    connect(reply, &QNetworkReply::downloadProgress, this, &SAKUpdateDownloader::setProgress);
}

void SAKUpdateDownloader::cancelDownload()
{
    httpRequestAborted = true;

    if (reply){
        reply->abort();
    }
}

QString SAKUpdateDownloader::tempPath()
{
    QString str = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    return str;
}

QFile* SAKUpdateDownloader::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
        qWarning() << "open failed" << file->errorString();
        return nullptr;
    }
    return file.take();
}

void SAKUpdateDownloader::downloadFinished()
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

    if (reply->error() != QNetworkReply::NoError) {
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

void SAKUpdateDownloader::read()
{
    if (file){
        file->write(reply->readAll());
    }
}

void SAKUpdateDownloader::setProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit updateProgress(bytesReceived, bytesTotal);
}
