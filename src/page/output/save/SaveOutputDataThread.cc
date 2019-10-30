/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include "SaveOutputDataThread.hh"
#include "SaveOutputDataSettings.hh"

#include <QFile>
#include <QTextStream>

SaveOutputDataThread::SaveOutputDataThread(QObject *parent)
    :QThread (parent)
{
    moveToThread(this);
}

void SaveOutputDataThread::writeDataToFile(QByteArray data, SaveOutputDataSettings::SaveOutputDataParamters parameters)
{
    if(parameters.fileName.isEmpty()){
        return;
    }

    QFile file(parameters.fileName);
    int format = parameters.format;
    QTextStream textStream(&file);
    switch (format) {
    case SaveOutputDataSettings::SaveOutputDataParamters::Bin:
        if (file.open(QFile::WriteOnly | QFile::Append)){
            file.write(data);
            file.close();
        }
        break;
    case SaveOutputDataSettings::SaveOutputDataParamters::Utf8:
        if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
            textStream << QString::fromUtf8(data) << QString("\n");
            file.close();
        }
        break;
    case SaveOutputDataSettings::SaveOutputDataParamters::Hex:
        if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
            textStream << QString(data.toHex(' ')) << QString("\n");
            file.close();
        }
        break;
    }
}

void SaveOutputDataThread::run()
{
    exec();
}
