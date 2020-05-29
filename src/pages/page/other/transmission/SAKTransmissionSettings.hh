/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
