/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "AutoResponseSettingPanel.h"
#include "ui_AutoResponseSettingPanel.h"

#include <QApplication>
#include <QDateTime>

AutoResponseSettingPanel::AutoResponseSettingPanel(QWidget *parent):
    QWidget(parent),
    ui(new Ui::AutoResponseSettingPanel),
    autoResponseItemWidget(new AutoResponseItem),
    clearOutputInfoTimer(new QTimer)
{
    ui->setupUi(this);
    ui->pushButtonInfoIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    ui->pushButtonInfoIcon->setIconSize(QSize(ui->pushButtonInfoIcon->width(), ui->pushButtonInfoIcon->height()));

    setWindowTitle(tr("自动回复设置"));
    setWindowModality(Qt::ApplicationModal);

    Connect();
}

AutoResponseSettingPanel::~AutoResponseSettingPanel()
{
    delete ui;
}

void AutoResponseSettingPanel::Connect()
{
    connect(ui->pushButtonAdd, SIGNAL(clicked(bool)), autoResponseItemWidget, SLOT(show()));
    connect(ui->pushButtonDelete, SIGNAL(clicked(bool)), this, SLOT(deleteAutoResponseItem()));
    connect(ui->pushButtonModify, SIGNAL(clicked(bool)), this, SLOT(modifyAutoResponseItem()));
    connect(autoResponseItemWidget, SIGNAL(need2addAotoResponseItem(QString,QString,QString)), this, SLOT(addAutoResponseItem(QString,QString,QString)));
    connect(autoResponseItemWidget, SIGNAL(need2modifyResponseItem(QString,QString,QString)), this, SLOT(modifyResponseItem(QString,QString,QString)));
    connect(clearOutputInfoTimer, SIGNAL(timeout()), this, SLOT(clearOutputInfo()));
}

void AutoResponseSettingPanel::addAutoResponseItem(QString receiveData, QString sendData, QString description)
{
    if (autoResponseItemList.length() == 9999){
        outputInfo(tr("自动回复数量达到最大值，终止操作！"), QString("red"));
        QApplication::beep();
        return;
    }

    QString itemToolTipString = packetItemString(receiveData, sendData);
    QString itemString = description;

    QListWidgetItem *listWidgetItem = new QListWidgetItem(QString("%1 ").arg(QString::number(autoResponseItemList.length() + 1), 4, '0') + itemString, ui->listWidgetAutoResponse);
    listWidgetItem->setToolTip(itemToolTipString);
    if (ui->listWidgetAutoResponse->currentRow() < 0){
        ui->listWidgetAutoResponse->setCurrentRow(0);
    }

    AutoResponseNode node;
    node.receiveDataString = receiveData;
    node.sendDataString = sendData;
    node.description = description;
    node.listWidgetItem = listWidgetItem;

    autoResponseItemList.append(node);
}

QString AutoResponseSettingPanel::packetItemString(QString receiveData, QString sendData)
{
    QString itemString;

    itemString.append(tr("接收数据："));
    itemString.append("\n");
    itemString.append(receiveData);
    itemString.append("\n");
    itemString.append(tr("回复数据："));
    itemString.append("\n");
    itemString.append(sendData);

    return itemString;
}

void AutoResponseSettingPanel::outputInfo(QString info, QString color)
{
    QString dateTimeInfo = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ");
    dateTimeInfo = QString("<font color=silver>%1<font>").arg(dateTimeInfo);

    ui->labelInfo->setText(QString("%1<font color=%2>%3</font>").arg(dateTimeInfo).arg(color).arg(info));

    /// 3秒后清空信息输出
    const int inventor = 3;
    if (clearOutputInfoTimer->isActive()){
        clearOutputInfoTimer->stop();
        clearOutputInfoTimer->start(inventor*1000);
    }else {
        clearOutputInfoTimer->start(inventor*1000);
    }
}

void AutoResponseSettingPanel::clearOutputInfo()
{
    ui->labelInfo->clear();
}

void AutoResponseSettingPanel::deleteAutoResponseItem()
{
    if (autoResponseItemList.isEmpty()){
        outputInfo(ERR_STR_LIST_IS_EMPTY, QString("red"));
        QApplication::beep();
        return;
    }else {
        QListWidgetItem *listWidgetItem = ui->listWidgetAutoResponse->currentItem();
        if ((listWidgetItem == NULL) || (listWidgetItem == nullptr)){
            return;
        }else {
            int index = ui->listWidgetAutoResponse->currentRow();
            ui->listWidgetAutoResponse->removeItemWidget(listWidgetItem);
            delete autoResponseItemList.at(index).listWidgetItem;
            autoResponseItemList.removeAt(index);

            resortTheAutoResponseItemList(ui->listWidgetAutoResponse->currentRow());
        }
    }
}

void AutoResponseSettingPanel::resortTheAutoResponseItemList(int currentRow)
{
    if (autoResponseItemList.isEmpty()){
        return;
    }else {
        for (int index = 0; index < ui->listWidgetAutoResponse->count(); index++){
            ui->listWidgetAutoResponse->setCurrentRow(index);
            ui->listWidgetAutoResponse->currentItem()->setText(QString("%1 ").arg(QString::number(index + 1), 4, '0') + autoResponseItemList.at(index).description);
            ui->listWidgetAutoResponse->currentItem()->setToolTip(packetItemString(autoResponseItemList.at(index).receiveDataString, autoResponseItemList.at(index).sendDataString));
        }

        if (currentRow < 0){
            ui->listWidgetAutoResponse->setCurrentRow(0);
        }else {
            ui->listWidgetAutoResponse->setCurrentRow(currentRow);
        }
    }
}

void AutoResponseSettingPanel::modifyAutoResponseItem()
{
    if (autoResponseItemList.isEmpty()){
        outputInfo(ERR_STR_LIST_IS_EMPTY, QString("red"));
        QApplication::beep();
        return;
    }else {
        int currentRow = ui->listWidgetAutoResponse->currentRow();
        need2modifyItemRow = currentRow;
        QString receiveDataString = autoResponseItemList.at(currentRow).receiveDataString;
        QString sendDataString = autoResponseItemList.at(currentRow).sendDataString;
        QString description = autoResponseItemList.at(currentRow).description;
        autoResponseItemWidget->setText(receiveDataString, sendDataString, description);
        autoResponseItemWidget->showModify();
    }
}

void AutoResponseSettingPanel::modifyResponseItem(QString receiveDataString, QString sendDataString, QString description)
{
    QListWidgetItem *listWidgetItem = ui->listWidgetAutoResponse->currentItem();
    if ((listWidgetItem == NULL) || (listWidgetItem == nullptr)){
        return;
    }else {
        listWidgetItem->setText(QString("%1 ").arg(QString::number(ui->listWidgetAutoResponse->currentRow() + 1), 4, '0') + description);
        listWidgetItem->setToolTip(packetItemString(receiveDataString, sendDataString));

        AutoResponseNode node;
        node.receiveDataString = receiveDataString;
        node.sendDataString = sendDataString;
        node.description = description;
        node.listWidgetItem = autoResponseItemList.at(need2modifyItemRow).listWidgetItem;
        autoResponseItemList.replace(need2modifyItemRow, node);
    }
}
