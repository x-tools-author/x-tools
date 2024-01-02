/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <Qt>

#include "sakinterface.h"
#include "sakstorertool.h"

SAKStorerTool::SAKStorerTool(QObject *parent)
    : SAKBaseTool{parent}
{}

SAKStorerTool::~SAKStorerTool()
{
    mInputContextListMutex.lock();
    mInputContextList.clear();
    mInputContextListMutex.unlock();
}

void SAKStorerTool::inputBytes(const QByteArray &bytes, const QVariant &context)
{
    if (enable()) {
        mInputContextListMutex.lock();
        mInputContextList.append({bytes, context});
        mInputContextListMutex.unlock();
    }
}

int SAKStorerTool::saveFormat()
{
    return mParameters.format;
}

void SAKStorerTool::setSaveFormat(int format)
{
    mParameters.format = format;
    emit saveFormatChanged();
}

bool SAKStorerTool::saveRx()
{
    return mParameters.saveRx;
}

void SAKStorerTool::setSaveRx(bool save)
{
    mParameters.saveRx = save;
    emit saveRxChanged();
}

bool SAKStorerTool::saveTx()
{
    return mParameters.saveTx;
}

void SAKStorerTool::setSaveTx(bool save)
{
    mParameters.saveTx = save;
    emit saveTxChanged();
}

bool SAKStorerTool::saveDate()
{
    return mParameters.saveDate;
}

void SAKStorerTool::setSaveDate(bool save)
{
    mParameters.saveDate = save;
    emit saveDateChanged();
}

bool SAKStorerTool::saveTime()
{
    return mParameters.saveTime;
}

void SAKStorerTool::setSaveTime(bool save)
{
    mParameters.saveTime = save;
    emit saveTimeChanged();
}

bool SAKStorerTool::saveMs()
{
    return mParameters.saveMs;
}

void SAKStorerTool::setSaveMs(bool save)
{
    mParameters.saveMs = save;
    emit saveMsChanged();
}

QString SAKStorerTool::fileName()
{
    return mParameters.file;
}

void SAKStorerTool::setFileName(const QString &name)
{
    mParameters.file = name;
    emit fileNameChanged();
}

void SAKStorerTool::run()
{
    QTimer *writeTimer = new QTimer();
    writeTimer->setInterval(2000);
    writeTimer->setSingleShot(true);
    connect(writeTimer, &QTimer::timeout, writeTimer, [=]() {
        this->write2file();
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
    QFile file(path);
    if (file.exists()) {
        if (file.size() > 1024 * 1024) {
            const QString format = QString("yyyy-MM-dd-hh-mm-ss_");
            auto dt = QDateTime::currentDateTime().toString(format);
            QDir dir(path);
            auto pathTemp = path;
            auto fileNameTemp = dir.dirName();
            pathTemp = pathTemp.remove(fileNameTemp);
            QString newFileName = pathTemp + dt + fileNameTemp;
            file.copy(newFileName);
            file.open(QFile::WriteOnly | QFile::Truncate);
            file.close();
        }
    } else {
        outputMessage(QtWarningMsg, QString("the file(%1) is not exist").arg(path));
    }

    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)) {
        QTextStream outStream(&file);
        while (!mInputContextList.isEmpty()) {
            auto ctx = mInputContextList.takeFirst();
            auto bytes = ctx.bytes;
            auto context = ctx.context;
            this->mParametersMutex.lock();
            int format = this->mParameters.format;
            this->mParametersMutex.unlock();
            auto str = SAKInterface::arrayToString(bytes, format);

            QString dtStr;
            auto dt = QDateTime::currentDateTime();
            if (mParameters.saveDate && mParameters.saveTime) {
                if (mParameters.saveMs) {
                    dtStr = dt.toString("yyyy-MM-dd hh:mm:ss.zzz ");
                } else {
                    dtStr = dt.toString("yyyy-MM-dd hh:mm:ss ");
                }
            } else {
                if (mParameters.saveDate) {
                    dtStr = dt.toString("yyyy-MM-dd ");
                } else {
                    if (mParameters.saveMs) {
                        dtStr = dt.toString("hh:mm:ss.zzz ");
                    } else {
                        if (mParameters.saveTime) {
                            dtStr = dt.toString("hh:mm:ss ");
                        }
                    }
                }
            }

            const QString txFlag = "Tx: ";
            const QString rxFlag = "Rx: ";
            QString flag = context.toJsonObject().value("flag").toString();
            flag = (flag == "rx" ? rxFlag : txFlag);

            if (flag == txFlag) {
                if (!mParameters.saveTx) {
                    continue;
                }
            }

            if (flag == rxFlag) {
                if (!mParameters.saveRx) {
                    continue;
                }
            }
            str = dtStr + flag + str;
            outStream << str << "\n";
        }
        file.close();
    } else {
        outputMessage(QtWarningMsg, file.errorString());
    }
}
