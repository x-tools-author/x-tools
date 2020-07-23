/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QFile>
#include <QEventLoop>
#include <QTextStream>

#include "SAKInterface.hh"
#include "SAKOutputSave2FileThread.hh"
#include "SAKOutputSave2FileDialog.hh"

SAKOutputSave2FileThread::SAKOutputSave2FileThread(QObject *parent)
    :QThread (parent)
{

}

SAKOutputSave2FileThread::~SAKOutputSave2FileThread()
{
    requestInterruption();
    threadWaitCondition.wakeAll();
    wait();
}

void SAKOutputSave2FileThread::writeDataToFile(QByteArray data, SAKOutputSave2FileDialog::SaveOutputDataParamters parameters)
{
    if(parameters.fileName.isEmpty()){
        return;
    }

    DataInfoStruct dataInfo;
    dataInfo.data = data;
    dataInfo.parameters = parameters;
    dataListMutex.lock();
    dataList.append(dataInfo);
    dataListMutex.unlock();

    threadWaitCondition.wakeAll();
}

void SAKOutputSave2FileThread::run()
{
    QEventLoop eventLoop;
    while (true) {
        /// @brief 响应外部中断
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 写数据
        while (true) {
            DataInfoStruct info = takeDataInfo();
            if (info.data.length()){
                innerWriteDataToFile(info.data, info.parameters);
            }else{
                break;
            }
        }

        /// @brief 事件输出
        eventLoop.processEvents();

        /// @brief 线程睡眠
        if (!isInterruptionRequested()){
            threadMutex.lock();
            threadWaitCondition.wait(&threadMutex, 100);
            threadMutex.unlock();
        }
    }
}

void SAKOutputSave2FileThread::innerWriteDataToFile(QByteArray data, SAKOutputSave2FileDialog::SaveOutputDataParamters parameters)
{
    QFile file(parameters.fileName);
    int format = parameters.format;
    QTextStream textStream(&file);
    switch (format) {
    case SAKOutputSave2FileDialog::SaveOutputDataParamters::Bin:
        if (file.open(QFile::WriteOnly | QFile::Append)){
            file.write(data);
            file.close();
        }
        break;
    case SAKOutputSave2FileDialog::SaveOutputDataParamters::Utf8:
        if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
            textStream << QString::fromUtf8(data) << QString("\n");
            file.close();
        }
        break;
    case SAKOutputSave2FileDialog::SaveOutputDataParamters::Hex:
        if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
            /// @brief  QByteArray::toHex(char separator)是Qt5.9中引入的
#if (QT_VERSION < QT_VERSION_CHECK(5,9,0))
            SAKInterface interface;
            textStream << QString(interface.byteArrayToHex(data, ' ')) << QString("\n");
#else
            textStream << QString(data.toHex(' ')) << QString("\n");
#endif
            file.close();
        }
        break;
    }
}

SAKOutputSave2FileThread::DataInfoStruct SAKOutputSave2FileThread::takeDataInfo()
{
    DataInfoStruct info;
    dataListMutex.lock();
    if (dataList.length()){
        info = dataList.takeFirst();
    }
    dataListMutex.unlock();

    return info;
}
