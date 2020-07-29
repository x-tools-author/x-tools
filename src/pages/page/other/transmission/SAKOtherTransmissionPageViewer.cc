/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QHBoxLayout>

#include "SAKDebugPage.hh"
#include "SAKOtherTransmissionPage.hh"
#include "SAKOtherTransmissionPageViewer.hh"

#include "ui_SAKTransmissionSettings.h"

SAKOtherTransmissionPageViewer::SAKOtherTransmissionPageViewer(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,_debugPage (debugPage)
    ,ui (new Ui::SAKOtherTransmissionPageViewer)
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

    serialPortTransmission = new SAKOtherTransmissionPage (debugPage, this);
    serialPortTransmission->setTransmissionType(SAKOtherTransmissionPage::SerialPortTransmission);
    udpTransmission = new SAKOtherTransmissionPage(debugPage, this);
    udpTransmission->setTransmissionType(SAKOtherTransmissionPage::UdpTransmission);
    tcpTransmission = new SAKOtherTransmissionPage(debugPage, this);
    tcpTransmission->setTransmissionType(SAKOtherTransmissionPage::TcpTransmission);

    installWidget(serialPortWidget, serialPortTransmission);
    installWidget(udpWidget, udpTransmission);
    installWidget(tcpWidget, tcpTransmission);        
}

SAKOtherTransmissionPageViewer::~SAKOtherTransmissionPageViewer()
{
    delete ui;

    delete udpTransmission;
    delete tcpTransmission;
    delete serialPortTransmission;
}
