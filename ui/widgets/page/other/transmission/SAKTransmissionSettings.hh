/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTRANSMISSIONSETTINGS_HH
#define SAKTRANSMISSIONSETTINGS_HH

#include <QWidget>
#include <QPushButton>
#include "SAKWidget.hh"

class SAKDebugPage;
class SAKTransmissionPage;

namespace Ui {
class SAKTransmissionSettings;
}

class SAKTransmissionSettings:public SAKWidget
{
    Q_OBJECT
public:
    SAKTransmissionSettings(SAKDebugPage *debugPage, QWidget *parent = nullptr);
    ~SAKTransmissionSettings();
private:
    QWidget                     *serialPortWidget       = nullptr;
    QWidget                     *udpWidget              = nullptr;
    QWidget                     *tcpWidget              = nullptr;
    SAKDebugPage                *_debugPage             = nullptr;
    Ui::SAKTransmissionSettings    *ui                     = nullptr;

    SAKTransmissionPage            *udpTransmission        = nullptr;
    SAKTransmissionPage            *tcpTransmission        = nullptr;
    SAKTransmissionPage            *serialPortTransmission = nullptr;
};

#endif
