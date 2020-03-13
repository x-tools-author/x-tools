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
#include <QHBoxLayout>

#include "SAKDebugPage.hh"
#include "SAKTransmissionPage.hh"
#include "SAKTransmissionSettings.hh"

#include "ui_SAKTransmissionSettings.h"

SAKTransmissionSettings::SAKTransmissionSettings(SAKDebugPage *debugPage, QWidget *parent)
    :SAKWidget (parent)
    ,_debugPage (debugPage)
    ,ui (new Ui::SAKTransmissionSettings)
{
    ui->setupUi(this);
    setWindowTitle(tr("数据转发设置"));

    serialPortWidget    = ui->serialPortWidget;
    udpWidget           = ui->udpWidget;
    tcpWidget           = ui->tcpWidget;

    auto installWidget = [](QWidget *tab, QWidget *page){
        QHBoxLayout *layout = new QHBoxLayout(tab);
        tab->setLayout(layout);
        layout->addWidget(page);
    };

    serialPortTransmission = new SAKTransmissionPage (debugPage, this);
    serialPortTransmission->setTransmissionType(SAKTransmissionPage::SerialPortTransmission);
    udpTransmission = new SAKTransmissionPage(debugPage, this);
    udpTransmission->setTransmissionType(SAKTransmissionPage::UdpTransmission);
    tcpTransmission = new SAKTransmissionPage(debugPage, this);
    tcpTransmission->setTransmissionType(SAKTransmissionPage::TcpTransmission);

    installWidget(serialPortWidget, serialPortTransmission);
    installWidget(udpWidget, udpTransmission);
    installWidget(tcpWidget, tcpTransmission);        
}

SAKTransmissionSettings::~SAKTransmissionSettings()
{
    delete ui;

    delete udpTransmission;
    delete tcpTransmission;
    delete serialPortTransmission;
}
