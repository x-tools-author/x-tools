/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QFile>
#include <QTextStream>

#include "SAKInterface.hh"
#include "SAKSaveOutputDataThread.hh"
#include "SAKSaveOutputDataSettings.hh"

SAKSaveOutputDataThread::SAKSaveOutputDataThread(QObject *parent)
    :QThread (parent)
{
    moveToThread(this);
}

void SAKSaveOutputDataThread::writeDataToFile(QByteArray data, SAKSaveOutputDataSettings::SaveOutputDataParamters parameters)
{
    if(parameters.fileName.isEmpty()){
        return;
    }

    QFile file(parameters.fileName);
    int format = parameters.format;
    QTextStream textStream(&file);
    switch (format) {
    case SAKSaveOutputDataSettings::SaveOutputDataParamters::Bin:
        if (file.open(QFile::WriteOnly | QFile::Append)){
            file.write(data);
            file.close();
        }
        break;
    case SAKSaveOutputDataSettings::SaveOutputDataParamters::Utf8:
        if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
            textStream << QString::fromUtf8(data) << QString("\n");
            file.close();
        }
        break;
    case SAKSaveOutputDataSettings::SaveOutputDataParamters::Hex:
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

void SAKSaveOutputDataThread::run()
{
    exec();
}
