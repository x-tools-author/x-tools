/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <Qt>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

#include "SAKStorerTool.hh"
#include "common/SAKInterface.hpp"

SAKStorerTool::SAKStorerTool(QObject *parent)
    : SAKBaseTool{"SAK.StorerTool", parent}
{

}

SAKStorerTool::~SAKStorerTool()
{
    mInputContextListMutex.lock();
    mInputContextList.clear();
    mInputContextListMutex.unlock();
}

void SAKStorerTool::inputBytes(const QByteArray &bytes,
                              const QVariant &context)
{
    if (enable()) {
        mInputContextListMutex.lock();
        mInputContextList.append({bytes, context});
        mInputContextListMutex.unlock();
    }
}

void SAKStorerTool::setPath(const QString &path)
{
    mParameters.file = path;
    mParameters.file = mParameters.file.remove("file:///");
}

void SAKStorerTool::setFormat(int format)
{
    mParameters.format = format;
}

void SAKStorerTool::setSaveDate(bool saveDate)
{
    mParameters.saveDate = saveDate;
}

void SAKStorerTool::setSaveTime(bool saveTime)
{
    mParameters.saveTime = saveTime;
}

void SAKStorerTool::setSaveMs(bool saveMs)
{
    mParameters.saveMs = saveMs;
}

void SAKStorerTool::run()
{
    QTimer *writeTimer = new QTimer();
    writeTimer->setInterval(2000);
    writeTimer->setSingleShot(true);
    connect(writeTimer, &QTimer::timeout, writeTimer, [=](){
        write2file();
        writeTimer->start();
    });
    writeTimer->start();

    exec();

    writeTimer->stop();
    writeTimer->deleteLater();
    writeTimer = nullptr;
    write2file();
    mInputContextList.clear();
}

void SAKStorerTool::write2file()
{
    if (mParameters.file.isEmpty()) {
        mInputContextList.clear();
        return;
    }

    QString path = mParameters.file;
    QFile file (path);
    if (file.exists()) {
        if (file.size() > 1024*1024) {
            const QString format = QString("yyyy-MM-dd-hh-mm-ss_");
            auto dt = QDateTime::currentDateTime().toString(format);
            QDir dir(path);
            auto pathTemp = path;
            auto fileNameTemp = dir.dirName();
            pathTemp = pathTemp.remove(fileNameTemp);
            QString newFileName = pathTemp + dt + fileNameTemp;
            file.copy(newFileName);
            file.open(QFile::WriteOnly|QFile::Truncate);
            file.close();
        }
    } else {
        outputMessage(QtWarningMsg,
                      QString("the file(%1) is not exist").arg(path));
    }

    if (file.open(QFile::WriteOnly|QFile::Text|QFile::Append)) {
        QTextStream outStream(&file);
        while (!mInputContextList.isEmpty()) {
            auto ctx = mInputContextList.takeFirst();
            auto bytes = ctx.bytes;
            auto context = ctx.context;
            auto str = SAKInterface::array2String(bytes, mParameters.format);

            QString dtStr;
            auto dt = QDateTime::currentDateTime();
            if (mParameters.saveDate && mParameters.saveTime) {
                if (mParameters.saveMs) {
                    dtStr = dt.toString("yyyy-MM-dd hh:mm:ss.zzz ");
                } else {
                    dtStr = dt.toString("yyyy-MM-dd hh:mm:ss ");
                }
            } else if (mParameters.saveDate) {
                dtStr = dt.toString("yyyy-MM-dd ");
            } else if (mParameters.saveTime) {
                if (mParameters.saveMs) {
                    dtStr = dt.toString("hh:mm:ss.zzz ");
                } else {
                    dtStr = dt.toString("hh:mm:ss ");
                }
            }

            QString flag = context.toJsonObject().value("isRx").toBool() ? "Rx: " : "Tx: ";
            str = dtStr + flag + str;
            outStream << str << Qt::endl;
        }
        file.close();
    } else {
        outputMessage(QtWarningMsg, file.errorString());
    }
}
