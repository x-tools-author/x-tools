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
 */
#ifndef TRANSMISSIONSETTINGS_HH
#define TRANSMISSIONSETTINGS_HH

#include <QWidget>
#include <QPushButton>
#include "SAKWidget.hh"

class SAKDebugPage;
class TransmissionPage;

namespace Ui {
class TransmissionSettings;
}

class TransmissionSettings:public SAKWidget
{
    Q_OBJECT
public:
    TransmissionSettings(SAKDebugPage *debugPage, QWidget *parent = nullptr);
    ~TransmissionSettings();
private:
    QWidget                     *serialPortWidget       = nullptr;
    QWidget                     *udpWidget              = nullptr;
    QWidget                     *tcpWidget              = nullptr;
    SAKDebugPage                *_debugPage             = nullptr;
    Ui::TransmissionSettings    *ui                     = nullptr;

    TransmissionPage            *udpTransmission        = nullptr;
    TransmissionPage            *tcpTransmission        = nullptr;
    TransmissionPage            *serialPortTransmission = nullptr;
};

#endif
