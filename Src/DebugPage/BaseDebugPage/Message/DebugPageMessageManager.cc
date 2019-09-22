/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
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
#include "SAKDebugPage.hh"
#include "DebugPageMessageManager.hh"

#include <QDateTime>

DebugPageMessageManager::DebugPageMessageManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,debugPage (debugPage)
{
    infoLabel = this->debugPage->infoLabel;
}

void DebugPageMessageManager::outputMessage(QString msg, bool isInfoMsg)
{
//    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    time = QString("<font color=silver>%1</font>").arg(time);
//    infoLabel->append(time);

//    if (isInfoMsg){
//#if 0
//        msg = QString("<font color=blue>%1</font>").arg(msg);
//#endif
//    }else{
//        msg = QString("<font color=red>%1</font>").arg(msg);
//        QApplication::beep();
//    }

//    infoLabel->append(msg);
//    infoLabel->append("");
}
