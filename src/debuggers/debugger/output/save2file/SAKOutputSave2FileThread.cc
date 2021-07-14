/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QEventLoop>
#include <QTextStream>

#include "SAKOutputSave2FileThread.hh"
#include "SAKOutputSave2FileDialog.hh"

SAKOutputSave2FileThread::SAKOutputSave2FileThread(QObject *parent)
    :QThread (parent)
{

}

SAKOutputSave2FileThread::~SAKOutputSave2FileThread()
{
    // flush temp data
    mThreadWaitCondition.wakeAll();
    // exit thread
    requestInterruption();
    mThreadWaitCondition.wakeAll();
    wait();
}

void SAKOutputSave2FileThread::writeDataToFile(QByteArray data, SAKOutputSave2FileDialog::ParametersContext parameters)
{
    if(parameters.fileName.isEmpty()){
        return;
    }

    DataInfoStruct dataInfo;
    dataInfo.data = data;
    dataInfo.parameters = parameters;
    mDataListMutex.lock();
    mDataList.append(dataInfo);
    mDataListMutex.unlock();

    // Wake the thread to write data
    mThreadWaitCondition.wakeAll();
}

void SAKOutputSave2FileThread::run()
{
    QEventLoop eventLoop;
    while (true) {
        // wirte data to file
        while (true) {
            DataInfoStruct info = takeDataInfo();
            if (info.data.length()){
                innerWriteDataToFile(info.data, info.parameters);
            }else{
                break;
            }
        }

        // emmmm...
        eventLoop.processEvents();

        // if no interruption requested, the thread will sleep
        if (isInterruptionRequested()){
            break;
        }else{
            mThreadMutex.lock();
            mThreadWaitCondition.wait(&mThreadMutex);
            mThreadMutex.unlock();
        }
    }
}

#include <QDebug>
void SAKOutputSave2FileThread::innerWriteDataToFile(QByteArray data, SAKOutputSave2FileDialog::ParametersContext parameters)
{
    // If the size of file more than 1M, create a new file to save data
    if (QFile::exists(parameters.fileName)){
        QFileInfo fileInfo(parameters.fileName);
        QString fullPath = parameters.fileName;
        QString fileName = fullPath.split('/').last();
        QString path = fullPath.remove(fileName);
        if (fileInfo.size() > 1024*1024){
            QFile file(parameters.fileName);
            file.rename(QString("%1/backup_%2_%3").arg(path).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")).arg(fileName));
        }
    }

    QFile file(parameters.fileName);
    int format = parameters.format;
    QTextStream textStream(&file);

    QString dataString;
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
        dataString = bytes2String(data, format);
        QString outString = QString("[%1%2]%3")
                .arg(parameters.saveTimestamp ? QString(QDateTime::currentDateTime().toString("hh:mm:ss ")) : QString(""))
                .arg(parameters.type == SAKOutputSave2FileDialog::ParametersContext::Read ? QString("Rx") : QString("Tx"))
                .arg(dataString);
        textStream << outString << "\n";
        file.close();
    }
}

SAKOutputSave2FileThread::DataInfoStruct SAKOutputSave2FileThread::takeDataInfo()
{
    DataInfoStruct info;
    mDataListMutex.lock();
    if (mDataList.length()){
        info = mDataList.takeFirst();
    }
    mDataListMutex.unlock();

    return info;
}

QString SAKOutputSave2FileThread::bytes2String(QByteArray bytes, int format)
{
    QString str;
    switch (format) {
    case SAKOutputSave2FileDialog::ParametersContext::Bin:
        for (int i = 0; i < bytes.length(); i++){
            QString temp = QString::number(int(bytes.at(i)), 2);
            str.append(temp + QString(" "));
        }
        str = str.trimmed();
        break;
    case SAKOutputSave2FileDialog::ParametersContext::Hex:
        for (int i = 0; i < bytes.length(); i++){
            QString temp = QString("%1").arg(QString::number(int(bytes.at(i)), 16), 2, '0');
            str.append(temp + QString(" "));
        }
        str = str.trimmed();
        break;
    case SAKOutputSave2FileDialog::ParametersContext::Utf8:
        str = QString::fromUtf8(bytes);
        break;
    default:
        str = tr("Unknown text format!");
        break;
    }

    return str;
}
