/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QApplication>
#include "SAKGlobal.hh"
#include "SAKOutputDataFactory.hh"

SAKOutputDataFactory::SAKOutputDataFactory(QObject *parent)
    :QThread (parent)
{
    moveToThread(this);
}

void SAKOutputDataFactory::run()
{
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKOutputDataFactory::terminate);
    exec();
}

void SAKOutputDataFactory::cookData(QByteArray rawData, SAKDebugPageOutputManager::OutputParameters parameters)
{
    QString str;

    str.append("<font color=silver>[</font>");

    if (parameters.showDate){
        str.append(QDate::currentDate().toString("yyyy-MM-dd "));
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.showTime){
        if (parameters.showMS){
            str.append(QTime::currentTime().toString("hh:mm:ss.zzz "));
        }else {
            str.append(QTime::currentTime().toString("hh:mm:ss "));
        }
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.isReceivedData){
        str.append("<font color=red>Rx</font>");
    }else {
        str.append("<font color=blue>Tx</font>");
    }
    str.append("<font color=silver>] </font>");

    if (parameters.textModel == SAKGlobal::Obin){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 2), 8, '0'));
        }
    }else if (parameters.textModel == SAKGlobal::Ooct){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 8), 3, '0'));
        }
    }else if (parameters.textModel == SAKGlobal::Odec){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 10)));
        }
    }else if (parameters.textModel == SAKGlobal::Ohex){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 16), 2, '0'));
        }
    }else if (parameters.textModel == SAKGlobal::Oascii){
        str.append(QString::fromLatin1(rawData));
    }else if (parameters.textModel == SAKGlobal::Outf8){
        str.append(QString::fromUtf8(rawData));
    }else if (parameters.textModel == SAKGlobal::Outf16){
        str.append(QString::fromUtf16(reinterpret_cast<const ushort*>(rawData.constData()),rawData.length()));
    }else if (parameters.textModel == SAKGlobal::Oucs4){
        str.append(QString::fromUcs4(reinterpret_cast<const char32_t*>(rawData.constData()),rawData.length()));
    }else if (parameters.textModel == SAKGlobal::Ostdwstring){
        str.append(QString::fromWCharArray(reinterpret_cast<const wchar_t*>(rawData.constData()),rawData.length()));
    }else if (parameters.textModel == SAKGlobal::Olocal){
        str.append(QString::fromLocal8Bit(rawData));
    }else {
        str.append(QString::fromUtf8(rawData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
    }

    emit dataCooked(str);
}
