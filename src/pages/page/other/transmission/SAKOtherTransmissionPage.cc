/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDateTime>

#include "SAKOtherTransmissionPage.hh"
#include "SAKOtherTransmissionItemUdp.hh"
#include "SAKOtherTransmissionItemTcp.hh"
#include "SAKOtherTransmissionItem.hh"
#include "SAKOtherTransmissionItemCom.hh"

#include "ui_SAKTransmissionPage.h"

SAKOtherTransmissionPage::SAKOtherTransmissionPage(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,_debugPage (debugPage)
    ,ui (new Ui::SAKOtherTransmissionPage)
{
    ui->setupUi(this);
    addItemPushButton = ui->addItemPushButton;
    deleteItemPushButton = ui->addItemPushButton;
    listWidget = ui->listWidget;
    infoLabel = ui->infoLabel;

    clearMessageInfoTimer.setInterval(5*1000);
    connect(&clearMessageInfoTimer, &QTimer::timeout, this, &SAKOtherTransmissionPage::clearMessage);
}

SAKOtherTransmissionPage::~SAKOtherTransmissionPage()
{
    delete ui;
}

void SAKOtherTransmissionPage::setTransmissionType(TransmissionType type)
{
    transmissionType = type;
}

void SAKOtherTransmissionPage::outputMessage(QString msg, bool isInfo)
{
    QString color = "black";
    if (!isInfo){
        color = "red";
        QApplication::beep();
    }
    infoLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));

    infoLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    clearMessageInfoTimer.start();
}

void SAKOtherTransmissionPage::clearMessage()
{
    clearMessageInfoTimer.stop();
    infoLabel->clear();
}

void SAKOtherTransmissionPage::on_addItemPushButton_clicked()
{    
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->insertItem(listWidget->count(), item);
    QWidget *itemWidget = Q_NULLPTR;
    switch (transmissionType){
    case SerialPortTransmission:
        itemWidget = new SAKOtherTransmissionItemCom(_debugPage, this);
        break;
    case UdpTransmission:
        itemWidget = new SAKOtherTransmissionItemUdp(_debugPage, this);
        break;
    case TcpTransmission:
        itemWidget = new SAKOtherTransmissionItemTcp(_debugPage, this);
        break;
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow transmissioin type");
        break;
    }
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    listWidget->setItemWidget(item, itemWidget);
    SAKOtherTransmissionItem *baseItemWidget = reinterpret_cast<SAKOtherTransmissionItem*>(itemWidget);
    connect(baseItemWidget, &SAKOtherTransmissionItem::requestOutputMessage, this, &SAKOtherTransmissionPage::outputMessage);
}

void SAKOtherTransmissionPage::on_deleteItemPushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    listWidget->removeItemWidget(item);
    delete item;
}
