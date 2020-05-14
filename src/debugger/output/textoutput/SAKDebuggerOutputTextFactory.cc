/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QDateTime>

#include "SAKDebugger.hh"
#include "SAKDebuggerOutputManager.hh"
#include "SAKDebuggerOutputSettings.hh"
#include "SAKDebuggerOutputTextFactory.hh"

SAKDebuggerOutputTextFactory::SAKDebuggerOutputTextFactory(QObject *parent)
    :QThread (parent)
{

}

SAKDebuggerOutputTextFactory::~SAKDebuggerOutputTextFactory()
{

}

void SAKDebuggerOutputTextFactory::run()
{
    while (1) {
        if (isInterruptionRequested()){
            return;
        }

        /// @brief 程序唤醒后处理所有已缓存的数据
        while (1){
            if (isInterruptionRequested()){
                return;
            }

            /// @brief 将array根据输出参数转换为string
            SAKDebuggerOutputSettings::ParametersContext paraCtx = outputSettings->parameters();
            TextContext textCtx = takeRawData();
            /// @brief 不处理空数据
            if (textCtx.text.length()){
                QString dateTimeString = prefixString(paraCtx.outputDate, paraCtx.outputTime, paraCtx.outputMs, textCtx.isRxData);
                QString dataString = dataToString(textCtx.text, paraCtx.outputFormat);
                QString cookedString = dateTimeString + QString(" ") + dataString;

                if ((paraCtx.outputTx && textCtx.isRxData) || ((!paraCtx.outputMs) && (!textCtx.isRxData))){
                    emit dataCooked(cookedString);
                }
            }else{
                break;
            }
        }

        /// @brief 每500毫秒自动唤醒处理数据
        threadMutex.lock();
        threadWaitCondition.wait(&threadMutex, 500);
        threadMutex.unlock();
    }
}

void SAKDebuggerOutputTextFactory::addRawData(QByteArray text, bool isRxData)
{
    textInfosListMutex.lock();
    TextContext textCtx{isRxData, text};
    textInfosList.append(textCtx);
    textInfosListMutex.unlock();

}

void SAKDebuggerOutputTextFactory::wakeMe()
{
    threadWaitCondition.wakeAll();
}

void SAKDebuggerOutputTextFactory::setOutputSettings(SAKDebuggerOutputSettings *settings)
{
    if (settings){
        outputSettings = settings;
        start();
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Oh, a null pointer");
    }
}

SAKDebuggerOutputTextFactory::TextContext SAKDebuggerOutputTextFactory::takeRawData()
{
    textInfosListMutex.lock();
    TextContext textCtx;
    if (textInfosList.length()){
        textCtx = textInfosList.takeFirst();
    }
    textInfosListMutex.unlock();

    return textCtx;
}

QString SAKDebuggerOutputTextFactory::prefixString(bool date, bool time, bool ms, bool isRxData)
{
    QString str;

    str.append("<font color=silver>[</font>");
    if (date){
        str.append(QDate::currentDate().toString("yyyy-MM-dd "));
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (time){
        if (ms){
            str.append(QTime::currentTime().toString("hh:mm:ss.zzz "));
        }else {
            str.append(QTime::currentTime().toString("hh:mm:ss "));
        }
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (isRxData){
        str.append("<font color=red>Rx</font>");
    }else {
        str.append("<font color=blue>Tx</font>");
    }
    str.append("<font color=silver>] </font>");

    return str;
}

QString SAKDebuggerOutputTextFactory::dataToString(QByteArray rawData, int textModel)
{
    QString str;
    if (textModel == SAKDebuggerOutputSettings::Bin){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 2), 8, '0'));
        }
    }else if (textModel == SAKDebuggerOutputSettings::Oct){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 8), 3, '0'));
        }
    }else if (textModel == SAKDebuggerOutputSettings::Dec){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 10)));
        }
    }else if (textModel == SAKDebuggerOutputSettings::Hex){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 16), 2, '0'));
        }
    }else if (textModel == SAKDebuggerOutputSettings::Ascii){
        str.append(QString::fromLatin1(rawData));
    }else if (textModel == SAKDebuggerOutputSettings::Utf8){
        str.append(QString::fromUtf8(rawData));
    }else if (textModel == SAKDebuggerOutputSettings::Utf16){
        str.append(QString::fromUtf16(reinterpret_cast<const ushort*>(rawData.constData()),rawData.length()));
    }else if (textModel == SAKDebuggerOutputSettings::Ucs4){
        str.append(QString::fromUcs4(reinterpret_cast<const char32_t*>(rawData.constData()),rawData.length()));
    }else if (textModel == SAKDebuggerOutputSettings::StdW){
        str.append(QString::fromWCharArray(reinterpret_cast<const wchar_t*>(rawData.constData()),rawData.length()));
    }else if (textModel == SAKDebuggerOutputSettings::System){
        str.append(QString::fromLocal8Bit(rawData));
    }else {
        str.append(QString::fromUtf8(rawData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
    }

    return str;
}
