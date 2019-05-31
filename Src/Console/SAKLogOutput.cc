/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#include "SAKLogOutput.hh"
#include "SAKGlobal.hh"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMetaEnum>
#include <QDebug>
#include <QApplication>

SAKLogOutput::SAKLogOutput(QObject *parent)
    :QThread (parent)
{
    moveToThread(this);
}

void SAKLogOutput::outputMessage(QString msg)
{
    QFile file(SAKGlobal::logFile());

    /*
     * 日志文件大于1M则清空
     */
    if (file.size() > 10240){
        file.open(QFile::ReadWrite | QFile::Truncate);
        file.close();
    }

    /*
     * 消息内容写入文件
     */
    if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text)){
        QTextStream outstream(&file);
        outstream << msg << endl;
        file.close();
    }else{
        QByteArray err("Can not open log file.");
        err.append(file.errorString().toUtf8());
        err.append(file.fileName().toUtf8());
        Q_ASSERT_X(false, __FUNCTION__, err.data());
    }
}

void SAKLogOutput::run()
{
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKLogOutput::destroyThis);
    exec();
}

void SAKLogOutput::destroyThis()
{
    this->exit();
    this->terminate();
    this->deleteLater();
}
