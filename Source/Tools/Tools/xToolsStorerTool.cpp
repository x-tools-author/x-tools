/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsStorerTool.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <Qt>

#include "xToolsDataStructure.h"

xToolsStorerTool::xToolsStorerTool(QObject *parent)
    : xToolsBaseTool{parent}
{}

xToolsStorerTool::~xToolsStorerTool()
{
    mInputContextListMutex.lock();
    mInputContextList.clear();
    mInputContextListMutex.unlock();
}

void xToolsStorerTool::inputBytes(const QByteArray &bytes)
{
    if (isEnable()) {
        mInputContextListMutex.lock();
        mInputContextList.append(bytes);
        mInputContextListMutex.unlock();
    }
}

int xToolsStorerTool::saveFormat()
{
    return mParameters.format;
}

void xToolsStorerTool::setSaveFormat(int format)
{
    mParameters.format = format;
    emit saveFormatChanged();
}

bool xToolsStorerTool::saveRx()
{
    return mParameters.saveRx;
}

void xToolsStorerTool::setSaveRx(bool save)
{
    mParameters.saveRx = save;
    emit saveRxChanged();
}

bool xToolsStorerTool::saveTx()
{
    return mParameters.saveTx;
}

void xToolsStorerTool::setSaveTx(bool save)
{
    mParameters.saveTx = save;
    emit saveTxChanged();
}

bool xToolsStorerTool::saveDate()
{
    return mParameters.saveDate;
}

void xToolsStorerTool::setSaveDate(bool save)
{
    mParameters.saveDate = save;
    emit saveDateChanged();
}

bool xToolsStorerTool::saveTime()
{
    return mParameters.saveTime;
}

void xToolsStorerTool::setSaveTime(bool save)
{
    mParameters.saveTime = save;
    emit saveTimeChanged();
}

bool xToolsStorerTool::saveMs()
{
    return mParameters.saveMs;
}

void xToolsStorerTool::setSaveMs(bool save)
{
    mParameters.saveMs = save;
    emit saveMsChanged();
}

QString xToolsStorerTool::fileName()
{
    return mParameters.file;
}

void xToolsStorerTool::setFileName(const QString &name)
{
    mParameters.file = name;
    emit fileNameChanged();
}

void xToolsStorerTool::run()
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

void xToolsStorerTool::write2file()
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
        qWarning() << QString("the file(%1) is not exist").arg(path);
    }

    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)) {
        QTextStream outStream(&file);
        while (!mInputContextList.isEmpty()) {
            auto bytes = mInputContextList.takeFirst();
            this->mParametersMutex.lock();
            int format = this->mParameters.format;
            this->mParametersMutex.unlock();
            auto str = xToolsDataStructure::byteArrayToString(bytes, format);
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
            outStream << dtStr << str << "\n";
        }
        file.close();
    } else {
        qWarning() << file.errorString();
    }
}
