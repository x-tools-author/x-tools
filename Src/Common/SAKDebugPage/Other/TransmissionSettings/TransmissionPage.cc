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
#include "TransmissionPage.hh"
#include "ui_TransmissionPage.h"
#include "UdpTransmissionItemWidget.hh"
#include "TcpTransmissionItemWidget.hh"
#include "BaseTransmissionItemWidget.hh"
#include "SerialPortTransmissionItemWidget.hh"

#include <QDateTime>

TransmissionPage::TransmissionPage(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,_debugPage (debugPage)
    ,ui (new Ui::TransmissionPage)
{
    ui->setupUi(this);
    addItemPushButton = ui->addItemPushButton;
    deleteItemPushButton = ui->addItemPushButton;
    listWidget = ui->listWidget;
    infoLabel = ui->infoLabel;

    clearMessageInfoTimer.setInterval(5*1000);
    connect(&clearMessageInfoTimer, &QTimer::timeout, this, &TransmissionPage::clearMessage);
}

TransmissionPage::~TransmissionPage()
{
    delete ui;
}

void TransmissionPage::setTransmissionType(TransmissionType type)
{
    transmissionType = type;
}

void TransmissionPage::outputMessage(QString msg, bool isInfo)
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

void TransmissionPage::clearMessage()
{
    clearMessageInfoTimer.stop();
    infoLabel->clear();
}

void TransmissionPage::on_addItemPushButton_clicked()
{    
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->insertItem(listWidget->count(), item);
    QWidget *itemWidget = nullptr;
    switch (transmissionType){
    case SerialPortTransmission:
        itemWidget = new SerialPortTransmissionItemWidget(_debugPage, this);
        break;
    case UdpTransmission:
        itemWidget = new UdpTransmissionItemWidget(_debugPage, this);
        break;
    case TcpTransmission:
        itemWidget = new TcpTransmissionItemWidget(_debugPage, this);
        break;
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow transmissioin type");
        break;
    }
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    listWidget->setItemWidget(item, itemWidget);
    BaseTransmissionItemWidget *baseItemWidget = reinterpret_cast<BaseTransmissionItemWidget*>(itemWidget);
    connect(baseItemWidget, &BaseTransmissionItemWidget::requestOutputMessage, this, &TransmissionPage::outputMessage);
}

void TransmissionPage::on_deleteItemPushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    listWidget->removeItemWidget(item);
    delete item;
}
