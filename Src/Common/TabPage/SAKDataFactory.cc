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
#include <QApplication>
#include "SAKDataFactory.hh"

SAKDataFactory::SAKDataFactory(QObject *parent)
    :QThread (parent)
{
    moveToThread(this);
}

void SAKDataFactory::run()
{
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKDataFactory::terminate);
    exec();
}

void SAKDataFactory::handleTheDataThatNeedsToBeSent(QString rawData, SAKTabPage::TextDisplayModel textModel)
{
    QByteArray data;
    if (textModel == SAKTabPage::Bin){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 2)));
        }
    }else if (textModel == SAKTabPage::Oct){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 8)));
        }
    }else if (textModel == SAKTabPage::Dec){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 10)));
        }
    }else if (textModel == SAKTabPage::Hex){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 16)));
        }
    }else if (textModel == SAKTabPage::Ascii){
        data = rawData.toLatin1();
    }else if (textModel == SAKTabPage::Local8bit){
        data = rawData.toLocal8Bit();
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow input mode");
    }

    emit sendBytes(data);
}

void SAKDataFactory::handleTheDataThatNeedsToBeOutputted(QByteArray data, SAKTabPage::OutputParameters parameters)
{

    QString str;

    str.append("[");

    if (parameters.showDate){
        str.append(QDate::currentDate().toString("yyyy/MM/dd "));
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

    if (parameters.textModel == SAKTabPage::Bin){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 2), 8, '0'));
        }
    }else if (parameters.textModel == SAKTabPage::Oct){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 8), 3, '0'));
        }
    }else if (parameters.textModel == SAKTabPage::Dec){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 10)));
        }
    }else if (parameters.textModel == SAKTabPage::Hex){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 16), 2, '0'));
        }
    }else if (parameters.textModel == SAKTabPage::Ascii){
        str.append(QString(data));
    }else if (parameters.textModel == SAKTabPage::Local8bit){
        str.append(QString::fromLocal8Bit(data));
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
    }

    emit outputData(str, parameters.isReceivedData);
}
