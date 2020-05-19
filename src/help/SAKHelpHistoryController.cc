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
#include <QFile>
#include <QDebug>
#include "SAKHelpHistoryController.hh"

SAKHelpHistoryController::SAKHelpHistoryController(QObject *parent)
    :QObject (parent)
{

}

SAKHelpHistoryController::~SAKHelpHistoryController()
{

}

QString SAKHelpHistoryController::readinHistoryInfo()
{
    QFile file(":/resources/files/History.txt");
    if (file.open(QFile::ReadOnly)){
        QByteArray array = file.readAll();
        file.close();
        QString str = QString::fromUtf8(array);
        return str;
    }else{
        qWarning() << "Can not open file!";
        return QString("Can not open file:%1").arg(file.errorString());
    }
}
