/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <Qt>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

#include "EDStorerTool.hpp"
#include "common/EDInterface.hpp"

EDStorerTool::EDStorerTool(QObject *parent)
    : EDBaseTool{"ED.StorerTool", parent}
{

}

EDStorerTool::~EDStorerTool()
{
    mInputContextListMutex.lock();
    mInputContextList.clear();
    mInputContextListMutex.unlock();
}

void EDStorerTool::inputBytes(const QByteArray &bytes,
                              const QJsonObject &context)
{
    if (enable()) {
        mInputContextListMutex.lock();
        mInputContextList.append({bytes, context});
        mInputContextListMutex.unlock();
    }
}

void EDStorerTool::setPath(const QString &path)
{
    mParameters.file = path;
    mParameters.file = mParameters.file.remove("file:///");
}

void EDStorerTool::setFormat(int format)
{
    mParameters.format = format;
}

void EDStorerTool::setSaveDate(bool saveDate)
{
    mParameters.saveDate = saveDate;
}

void EDStorerTool::setSaveTime(bool saveTime)
{
    mParameters.saveTime = saveTime;
}

void EDStorerTool::setSaveMs(bool saveMs)
{
    mParameters.saveMs = saveMs;
}

bool EDStorerTool::initialize(QString &errStr)
{
    mWriteTimer = new QTimer();
    mWriteTimer->setInterval(2000);
    mWriteTimer->setSingleShot(true);
    connect(mWriteTimer, &QTimer::timeout, mWriteTimer, [=](){
        emit invokeOutputBytes(EDPrivateSignal{});
    });
    mWriteTimer->start();

    return true;
}

void EDStorerTool::outputBytesHandler()
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
            auto str = EDInterface::array2String(bytes, mParameters.format);

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

            QString flag = context.value("isRx").toBool() ? "Rx: " : "Tx: ";
            str = dtStr + flag + str;
            outStream << str << Qt::endl;
        }
        file.close();

        if (mWriteTimer) {
            mWriteTimer->start();
        }
    } else {
        outputMessage(QtWarningMsg, file.errorString());
    }
}

void EDStorerTool::uninitialize()
{
    if (mWriteTimer) {
        mWriteTimer->stop();
        mWriteTimer->deleteLater();
        mWriteTimer = nullptr;
    }

    outputBytesHandler();
    mInputContextList.clear();
}
