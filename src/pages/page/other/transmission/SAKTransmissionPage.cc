/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDateTime>

#include "SAKTransmissionPage.hh"
#include "SAKUdpTransmissionItemWidget.hh"
#include "SAKTcpTransmissionItemWidget.hh"
#include "SAKBaseTransmissionItemWidget.hh"
#include "SAKSerialPortTransmissionItemWidget.hh"

#include "ui_SAKTransmissionPage.h"

SAKTransmissionPage::SAKTransmissionPage(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,_debugPage (debugPage)
    ,ui (new Ui::SAKTransmissionPage)
{
    ui->setupUi(this);
    addItemPushButton = ui->addItemPushButton;
    deleteItemPushButton = ui->addItemPushButton;
    listWidget = ui->listWidget;
    infoLabel = ui->infoLabel;

    clearMessageInfoTimer.setInterval(5*1000);
    connect(&clearMessageInfoTimer, &QTimer::timeout, this, &SAKTransmissionPage::clearMessage);
}

SAKTransmissionPage::~SAKTransmissionPage()
{
    delete ui;
}

void SAKTransmissionPage::setTransmissionType(TransmissionType type)
{
    transmissionType = type;
}

void SAKTransmissionPage::outputMessage(QString msg, bool isInfo)
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

void SAKTransmissionPage::clearMessage()
{
    clearMessageInfoTimer.stop();
    infoLabel->clear();
}

void SAKTransmissionPage::on_addItemPushButton_clicked()
{    
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->insertItem(listWidget->count(), item);
    QWidget *itemWidget = Q_NULLPTR;
    switch (transmissionType){
    case SerialPortTransmission:
        itemWidget = new SAKSerialPortTransmissionItemWidget(_debugPage, this);
        break;
    case UdpTransmission:
        itemWidget = new SAKUdpTransmissionItemWidget(_debugPage, this);
        break;
    case TcpTransmission:
        itemWidget = new SAKTcpTransmissionItemWidget(_debugPage, this);
        break;
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow transmissioin type");
        break;
    }
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    listWidget->setItemWidget(item, itemWidget);
    SAKBaseTransmissionItemWidget *baseItemWidget = reinterpret_cast<SAKBaseTransmissionItemWidget*>(itemWidget);
    connect(baseItemWidget, &SAKBaseTransmissionItemWidget::requestOutputMessage, this, &SAKTransmissionPage::outputMessage);
}

void SAKTransmissionPage::on_deleteItemPushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    listWidget->removeItemWidget(item);
    delete item;
}
