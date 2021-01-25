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

    connect(mSerialPortTransmission, &SAKOtherTransmissionPage::invokeClose, this, &SAKOtherTransmissionPageViewer::close);
    connect(mUdpTransmission, &SAKOtherTransmissionPage::invokeClose, this, &SAKOtherTransmissionPageViewer::close);
    connect(mTcpTransmission, &SAKOtherTransmissionPage::invokeClose, this, &SAKOtherTransmissionPageViewer::close);

    installWidget(mSerialPortWidget, mSerialPortTransmission);
    installWidget(mUdpWidget, mUdpTransmission);
    installWidget(mTcpWidget, mTcpTransmission);

    // Readin settings
    const QString settingsKeyCurrentPageIndex = QString("%1/CurrentTransmissionPageIndex").arg(mDebugPage->settingsGroup());
    auto settings = mDebugPage->settings();
    auto indexString = settings->value(settingsKeyCurrentPageIndex).toString();
    if (indexString.length()){
        auto index = indexString.toInt();
        if (index < mUi->tabWidget->count()){
            mUi->tabWidget->setCurrentIndex(index);
        }
    }

    // Update current page index when index changed
    connect(mUi->tabWidget, &QTabWidget::currentChanged, this, [=](const int index){
        settings->setValue(settingsKeyCurrentPageIndex, index);
    });
}

SAKOtherTransmissionPageViewer::~SAKOtherTransmissionPageViewer()
{
    delete mUi;

    delete mUdpTransmission;
    delete mTcpTransmission;
    delete mSerialPortTransmission;
}
