/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
