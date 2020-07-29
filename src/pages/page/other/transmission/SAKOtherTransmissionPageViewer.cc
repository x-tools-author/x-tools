/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QHBoxLayout>

#include "SAKDebugPage.hh"
#include "SAKTransmissionPage.hh"
#include "SAKTransmissionSettings.hh"

#include "ui_SAKTransmissionSettings.h"

SAKTransmissionSettings::SAKTransmissionSettings(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
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
