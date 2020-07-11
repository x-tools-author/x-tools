/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTRANSMISSIONSETTINGS_HH
#define SAKTRANSMISSIONSETTINGS_HH

#include <QWidget>
#include <QPushButton>

class SAKDebugPage;
class SAKTransmissionPage;

namespace Ui {
class SAKTransmissionSettings;
}

class SAKTransmissionSettings:public QWidget
{
    Q_OBJECT
public:
    SAKTransmissionSettings(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTransmissionSettings();
private:
    QWidget                     *serialPortWidget       = Q_NULLPTR;
    QWidget                     *udpWidget              = Q_NULLPTR;
    QWidget                     *tcpWidget              = Q_NULLPTR;
    SAKDebugPage                *_debugPage             = Q_NULLPTR;
    Ui::SAKTransmissionSettings    *ui                     = Q_NULLPTR;

    SAKTransmissionPage            *udpTransmission        = Q_NULLPTR;
    SAKTransmissionPage            *tcpTransmission        = Q_NULLPTR;
    SAKTransmissionPage            *serialPortTransmission = Q_NULLPTR;
};

#endif
