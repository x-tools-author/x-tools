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

#include "ui_SAKOtherTransmissionPageViewer.h"

SAKOtherTransmissionPageViewer::SAKOtherTransmissionPageViewer(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage (debugPage)
    ,mUi (new Ui::SAKOtherTransmissionPageViewer)
{
    mUi->setupUi(this);

    mSerialPortWidget = mUi->serialPortWidget;
    mUdpWidget = mUi->udpWidget;
    mTcpWidget = mUi->tcpWidget;

    auto installWidget = [](QWidget *tab, QWidget *page){
        QHBoxLayout *layout = new QHBoxLayout(tab);
        tab->setLayout(layout);
        layout->addWidget(page);
    };

    mSerialPortTransmission = new SAKOtherTransmissionPage (debugPage, this);
    mSerialPortTransmission->setTransmissionType(SAKOtherTransmissionPage::SerialPortTransmission);
    mUdpTransmission = new SAKOtherTransmissionPage(debugPage, this);
    mUdpTransmission->setTransmissionType(SAKOtherTransmissionPage::UdpTransmission);
    mTcpTransmission = new SAKOtherTransmissionPage(debugPage, this);
    mTcpTransmission->setTransmissionType(SAKOtherTransmissionPage::TcpTransmission);

    installWidget(mSerialPortWidget, mSerialPortTransmission);
    installWidget(mUdpWidget, mUdpTransmission);
    installWidget(mTcpWidget, mTcpTransmission);
}

SAKOtherTransmissionPageViewer::~SAKOtherTransmissionPageViewer()
{
    delete mUi;

    delete mUdpTransmission;
    delete mTcpTransmission;
    delete mSerialPortTransmission;
}
