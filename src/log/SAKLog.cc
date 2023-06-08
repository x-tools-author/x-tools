/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDir>
#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QDateTime>
#include <QFileInfoList>
#include <QStandardPaths>

#include "SAKLog.hh"
#include "SAKSettings.hh"

QVector<SAKLog::LogContext> SAKLog::mLogContextVector;
QMutex SAKLog::mLogContextVectorMutex;
SAKLog::SAKLog(QObject *parent)
    : QThread{parent}
{
    QString fileName = SAKSettings::instance()->fileName();
    QUrl url(fileName);
    QString path = fileName;
    mLogPath = path.remove(url.fileName());
    mLogPath += "log";

    QDir dir(mLogPath);
    if (!dir.exists()) {
        if (dir.mkdir(mLogPath)) {
            qCInfo(mLoggingCategory) << "Create log dir successfully.";
        } else {
            qCInfo(mLoggingCategory) << "Create log dir failed!";
        }
    } else {
        qCInfo(mLoggingCategory) << "The log dir is exists,"
                                    " need not to create.";
    }

    if (!SAKSettings::instance()->contains(mSettingsKey.logFileLifeCycle)) {
        setLogLifeCycle(30);
    }

    mLogLifeCycle = logLifeCycle();
}

SAKLog::~SAKLog()
{
    qCDebug(mLoggingCategory) << __FUNCTION__;

    exit();
    wait();
}

qint64 logLifeCycle();
void setLogLifeCycle(qint64 t);

void SAKLog::messageOutput(QtMsgType type, const QMessageLogContext &context,
                           const QString &msg)
{
    Q_UNUSED(context)
    LogContext ctx{type, QString(context.category), msg};
    mLogContextVectorMutex.lock();
    while (mLogContextVector.count() > 1024) {
        mLogContextVector.removeFirst();
    }
    mLogContextVector.append(ctx);
    mLogContextVectorMutex.unlock();
}

SAKLog *SAKLog::instance()
{
    static SAKLog log;
    return &log;
}

qint64 SAKLog::logLifeCycle()
{
    const QString key = mSettingsKey.logFileLifeCycle;
    qint64 ret = SAKSettings::instance()->value(key).toULongLong();
    if (ret < 1) {
        ret = 1;
        qCWarning(mLoggingCategory) << "The life cycle is too short,"
                                       " it has been set to 1 day.";
    }

    if (ret > 30) {
        ret = 30;
        qCWarning(mLoggingCategory) << "The life cycle is too long,"
                                       " it has been set to 30 days.";
    }

    return ret;
}

void SAKLog::setLogLifeCycle(qint64 t)
{
    const QString key = mSettingsKey.logFileLifeCycle;
    SAKSettings::instance()->setValue(key, t);
}

void SAKLog::run()
{
    QTimer *writeTimer = new QTimer();
    writeTimer->setInterval(1000);
    writeTimer->setSingleShot(true);
    connect(writeTimer, &QTimer::timeout, writeTimer, [=](){
        writeLog();
        writeTimer->start();
    });

    QTimer *clearTimer = new QTimer();
    clearTimer->setInterval(10*1000);
    clearTimer->setSingleShot(true);
    connect(clearTimer, &QTimer::timeout, clearTimer, [=](){
        clearLog();
        clearTimer->start();
    });

    writeTimer->start();
    clearTimer->start();
    exec();

    writeTimer->stop();
    writeTimer->deleteLater();
    writeTimer = nullptr;

    clearTimer->stop();
    clearTimer->deleteLater();
    clearTimer = nullptr;
}

void SAKLog::writeLog()
{
    mLogContextVectorMutex.lock();
    QVector<LogContext> logCtxVector = mLogContextVector;
    mLogContextVector.clear();
    mLogContextVectorMutex.unlock();

    if (logCtxVector.isEmpty()) {
        return;
    }

    const QDateTime dt = QDateTime::currentDateTime();
    const QString fileName = mLogPath + "/sak_log_" + dt.toString("yyyy_MM_dd");
    const QString suffix = ".log";
    QFile file(fileName + suffix);
    if (file.exists() && file.size() >= 1024*1024) {
        QString newName = fileName + dt.toString("_hh_mm_ss") + suffix;
        QFile::rename(fileName, newName);
    }

    if (file.open(QFile::WriteOnly|QFile::Text|QFile::Append)) {
        QTextStream out(&file);
        for (auto &logCtx : logCtxVector) {
            QString flag = "[U]";
            if (logCtx.type == QtDebugMsg) {
                flag = "[D]";
            } else if (logCtx.type == QtInfoMsg) {
                flag = "[I]";
            } else if (logCtx.type == QtWarningMsg) {
                flag = "[W]";
            } else if (logCtx.type == QtCriticalMsg) {
                flag = "[C]";
            } else if (logCtx.type == QtFatalMsg) {
                flag = "[F]";
            }

            out << flag << " " << logCtx.category << logCtx.msg << "\n";
        }

        file.close();
    } else {
        QStringList list;
        list << file.errorString() << " > " << "lor_error.log";
        QProcess::startDetached("echo", list);
    }
}

void SAKLog::clearLog()
{
    QDir dir(mLogPath);
    QFileInfoList infoList = dir.entryInfoList(QDir::NoDotAndDotDot);
    for (auto &info : infoList) {
        qint64 dateTime = info.birthTime().toMSecsSinceEpoch();
        qint64 currentDateTime = QDateTime::currentMSecsSinceEpoch();
        quint64 interval = qAbs(mLogLifeCycle*24*60*60*1000);
        if (quint64(qAbs(dateTime - currentDateTime)) > interval) {
            QFile::remove(info.fileName());
            qCInfo(mLoggingCategory) << "Remmove log file:" << info.fileName();
        }
    }
}
