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
#include <QFileDialog>

#include "SAKOtherTransmissionPage.hh"
#include "SAKOtherTransmissionItemUdp.hh"
#include "SAKOtherTransmissionItemTcp.hh"
#include "SAKOtherTransmissionItem.hh"
#include "SAKOtherTransmissionItemCom.hh"

#include "ui_SAKOtherTransmissionPage.h"

SAKOtherTransmissionPage::SAKOtherTransmissionPage(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,mDebugPage (debugPage)
    ,mUi (new Ui::SAKOtherTransmissionPage)
{
    mUi->setupUi(this);
    mAddItemPushButton = mUi->addItemPushButton;
    mDeleteItemPushButton = mUi->addItemPushButton;
    mListWidget = mUi->listWidget;
    mInfoLabel = mUi->infoLabel;

    mClearMessageInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearMessageInfoTimer, &QTimer::timeout, this, &SAKOtherTransmissionPage::clearMessage);
}

SAKOtherTransmissionPage::~SAKOtherTransmissionPage()
{
    delete mUi;
}

void SAKOtherTransmissionPage::setTransmissionType(TransmissionType type)
{
    mTransmissionType = type;
}

void SAKOtherTransmissionPage::import(const QString fileName)
{
    Q_UNUSED(fileName);
}

void SAKOtherTransmissionPage::outport(const QString fileName)
{
    Q_UNUSED(fileName);
}

void SAKOtherTransmissionPage::outputMessage(QString msg, bool isInfo)
{
    QString color = "black";
    if (!isInfo){
        color = "red";
        QApplication::beep();
    }
    mInfoLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));

    mInfoLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    mClearMessageInfoTimer.start();
}

void SAKOtherTransmissionPage::clearMessage()
{
    mClearMessageInfoTimer.stop();
    mInfoLabel->clear();
}

void SAKOtherTransmissionPage::on_addItemPushButton_clicked()
{    
    QListWidgetItem *item = new QListWidgetItem(mListWidget);
    mListWidget->insertItem(mListWidget->count(), item);
    QWidget *itemWidget = Q_NULLPTR;
    switch (mTransmissionType){
    case SerialPortTransmission:
        itemWidget = new SAKOtherTransmissionItemCom(mDebugPage, this);
        break;
    case UdpTransmission:
        itemWidget = new SAKOtherTransmissionItemUdp(mDebugPage, this);
        break;
    case TcpTransmission:
        itemWidget = new SAKOtherTransmissionItemTcp(mDebugPage, this);
        break;
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknown transmissioin type");
        break;
    }

    if (itemWidget){
        item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
        mListWidget->setItemWidget(item, itemWidget);
        SAKOtherTransmissionItem *baseItemWidget = reinterpret_cast<SAKOtherTransmissionItem*>(itemWidget);
        connect(baseItemWidget, &SAKOtherTransmissionItem::requestOutputMessage, this, &SAKOtherTransmissionPage::outputMessage);
    }
}

void SAKOtherTransmissionPage::on_deleteItemPushButton_clicked()
{
    QListWidgetItem *item = mListWidget->currentItem();
    mListWidget->removeItemWidget(item);
    delete item;
}

void SAKOtherTransmissionPage::on_closePushButton_clicked()
{
    emit invokeClose();
}

void SAKOtherTransmissionPage::on_importPushButton_clicked()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Import file"), "./", QString("Json (*.json)"));
    if (fileName.length()) {
        import(fileName);
    }
}

void SAKOtherTransmissionPage::on_outportPushButton_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Import file"), "./", QString("Json (*.json)"));
    if (fileName.length()) {
        outport(fileName);
    }
}
