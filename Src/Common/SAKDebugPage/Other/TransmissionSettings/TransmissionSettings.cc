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
#include <QHBoxLayout>
#include "SAKDebugPage.hh"
#include "TransmissionPage.hh"
#include "TransmissionSettings.hh"
#include "ui_TransmissionSettings.h"

TransmissionSettings::TransmissionSettings(SAKDebugPage *debugPage, QWidget *parent)
    :SAKWidget (parent)
    ,_debugPage (debugPage)
    ,ui (new Ui::TransmissionSettings)
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

    serialPortTransmission = new TransmissionPage (debugPage, this);
    serialPortTransmission->setTransmissionType(TransmissionPage::SerialPortTransmission);
    udpTransmission = new TransmissionPage(debugPage, this);
    udpTransmission->setTransmissionType(TransmissionPage::UdpTransmission);
    tcpTransmission = new TransmissionPage(debugPage, this);
    tcpTransmission->setTransmissionType(TransmissionPage::TcpTransmission);

    installWidget(serialPortWidget, serialPortTransmission);
    installWidget(udpWidget, udpTransmission);
    installWidget(tcpWidget, tcpTransmission);        
}

TransmissionSettings::~TransmissionSettings()
{
    delete ui;

    delete udpTransmission;
    delete tcpTransmission;
    delete serialPortTransmission;
}
