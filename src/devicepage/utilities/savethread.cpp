/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "savethread.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QLocale>
#include <QRegularExpression>
#include <QTimer>

#include "devicepage/common/xio.h"

SaveThread::SaveThread(QObject *parent)
    : QThread(parent)
{}

SaveThread::~SaveThread()
{
    exit();
    wait();
}

void SaveThread::saveData(const SaveParameters &parameters, const QByteArray &data, bool isRx)
{
    m_ctxListMutex.lock();
    SaveContext ctx;
    ctx.parameters = parameters;
    ctx.data = data;
    ctx.isRx = isRx;
    m_ctxList.append(ctx);
    m_ctxListMutex.unlock();
}

void saveDataToFile(const SaveThread::SaveContext &ctx, QFile *file)
{
    QDateTime now = QDateTime::currentDateTime();
    QString dateFmt = QLocale().dateFormat();
    QString timeFmt = QLocale().timeFormat(QLocale::ShortFormat);

    QString date = now.toString(dateFmt);
    QString time = now.toString(timeFmt);
    QString ms = QString::number(now.time().msec());
    QString text = xTools::xIO::bytes2string(ctx.data, ctx.parameters.format);

    QString line;
    line += ctx.isRx ? "RX " : "TX ";
    if (ctx.parameters.saveDate) {
        line += date + " ";
    }
    if (ctx.parameters.saveTime) {
        line += time + " ";
    }
    if (ctx.parameters.saveMs) {
        line += ms + " ";
    }

    static const QRegularExpression reg("[\\s]+");
    line.replace(reg, " ");
    line += text;
    QTextStream stream(file);
    stream << line << "\n";
}

void renameFile(const QString &oldName)
{
    QFileInfo info(oldName);
    QFile file(oldName);

    QString t = "_" + QString::number(QDateTime::currentMSecsSinceEpoch());
    QString newName = info.absolutePath() + "/" + info.baseName() + t + "." + info.completeSuffix();
    if (!file.rename(newName)) {
        qWarning() << "Failed to rename file" << oldName << "to" << newName;
    }
}

void saveDataToFile(const QList<SaveThread::SaveContext> &ctxList)
{
    QFile *file = nullptr;
    for (SaveThread::SaveContext const &ctx : ctxList) {
        if (!ctx.parameters.saveRx && ctx.isRx) {
            continue;
        }

        if (!ctx.parameters.saveTx && !ctx.isRx) {
            continue;
        }

        if (!ctx.parameters.saveDate) {
            continue;
        }

        if (ctx.parameters.fileName.isEmpty()) {
            continue;
        }

        if (ctx.data.isEmpty()) {
            return;
        }

        QFileInfo fileInfo(ctx.parameters.fileName);
        if (fileInfo.exists() && fileInfo.size() >= ctx.parameters.maxKBytes * 1024) {
            renameFile(ctx.parameters.fileName);
        }

        if (!file) {
            file = new QFile(ctx.parameters.fileName);
            if (!file->open(QIODevice::WriteOnly | QIODevice::Append)) {
                file->deleteLater();
                file = nullptr;
                continue;
            }
        }

        saveDataToFile(ctx, file);
    }

    if (file) {
        file->close();
        file->deleteLater();
        file = nullptr;
    }
}

void SaveThread::run()
{
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, timer, [this, timer]() {
        this->m_ctxListMutex.lock();
        QList<SaveContext> dataList = this->m_ctxList;
        this->m_ctxList.clear();
        this->m_ctxListMutex.unlock();

        saveDataToFile(dataList);
        timer->start();
    });

    timer->start();
    exec();

    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}
