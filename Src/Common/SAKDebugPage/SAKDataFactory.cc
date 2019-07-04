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
#include <QtEndian>
#include <QApplication>

#include "SAKDataFactory.hh"
#include "SAKCRCInterface.hh"

SAKDataFactory::SAKDataFactory(SAKDebugPage *page, QObject *parent)
    :QThread (parent)
    ,debugPage (page)
{
    moveToThread(this);
}

void SAKDataFactory::run()
{
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKDataFactory::terminate);
    exec();
}

void SAKDataFactory::handleTheDataThatNeedsToBeSent(QString rawData, SAKDebugPage::InputParameters parameters)
{
    QByteArray data = debugPage->cookedData(rawData);
    if (parameters.addCRC){
        uint32_t crc  = debugPage->crcCalculate(data, parameters.crcModel);
        uint8_t  crc8  = static_cast<uint8_t>(crc);
        uint16_t crc16 = static_cast<uint16_t>(crc);
        int bitsWidth = debugPage->crcInterface->getBitsWidth(parameters.crcModel);
        if (parameters.bigEnfian){
            crc16 = qToBigEndian(crc16);
            crc = qToBigEndian(crc);
        }

        switch (bitsWidth) {
        case 8:
            data.append(reinterpret_cast<char*>(&crc8), 1);
            break;
        case 16:
            data.append(reinterpret_cast<char*>(&crc16), 2);
            break;
        case 32:
            data.append(reinterpret_cast<char*>(&crc), 4);
            break;
        default:
            break;
        }
    }

    emit sendBytes(data);
}

void SAKDataFactory::handleTheDataThatNeedsToBeOutputted(QByteArray data, SAKDebugPage::OutputParameters parameters)
{

    QString str;

    str.append("[");

    if (parameters.showDate){
        str.append(QDate::currentDate().toString("yyyy-MM-dd "));
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.showTime){
        if (parameters.showMS){
            str.append(QTime::currentTime().toString("hh:mm:ss.z "));
        }else {
            str.append(QTime::currentTime().toString("hh:mm:ss "));
        }
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.isReceivedData){
        str.append("<font color=blue>Rx</font>");
    }else {
        str.append("<font color=purple>Tx</font>");
    }
    str.append("<font color=silver>] </font>");

    if (parameters.textModel == SAKDebugPage::Bin){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 2), 8, '0'));
        }
    }else if (parameters.textModel == SAKDebugPage::Oct){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 8), 3, '0'));
        }
    }else if (parameters.textModel == SAKDebugPage::Dec){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 10)));
        }
    }else if (parameters.textModel == SAKDebugPage::Hex){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 16), 2, '0'));
        }
    }else if (parameters.textModel == SAKDebugPage::Ascii){
        str.append(QString(data));
    }else if (parameters.textModel == SAKDebugPage::Local8bit){
        str.append(QString::fromLocal8Bit(data));
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
    }

    emit outputData(str, parameters.isReceivedData);
}
