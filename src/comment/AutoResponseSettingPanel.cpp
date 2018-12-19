/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "AutoResponseSettingPanel.h"
#include "ui_AutoResponseSettingPanel.h"

#include <QApplication>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QTextStream>
#include <QMessageBox>

#define ITEM_ROOTNAME       "QtSwissArmyKnife-jdflkasdkjfoasdnhfoijasdhfg"
#define ITEM_DESCRIPTION    "附加说明"
#define ITEM_RECEIVING_DATA "接收数据"
#define ITEM_SENDING_DATA   "回复数据"

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
    connect(autoResponseItemWidget, SIGNAL(need2modifyResponseItem(QString,QString,QString)), this, SLOT(modifyAutoResponseItem(QString,QString,QString)));
    connect(ui->pushButtonClear, SIGNAL(clicked(bool)), this, SLOT(clearAutoResponseItem()));
    connect(clearOutputInfoTimer, SIGNAL(timeout()), this, SLOT(clearOutputInfo()));
    connect(ui->pushButtonSaveAsFile, SIGNAL(clicked(bool)), this, SLOT(saveAsFile()));
    connect(ui->pushButtonReadInFile, SIGNAL(clicked(bool)), this, SLOT(readIn()));
    connect(ui->checkBoxAutoResponse, SIGNAL(clicked(bool)), this, SLOT(enableAutoResponseBtClicked()));
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
        if ((listWidgetItem == nullptr) || (listWidgetItem == nullptr)){
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

void AutoResponseSettingPanel::modifyAutoResponseItem(QString receiveDataString, QString sendDataString, QString description)
{
    QListWidgetItem *listWidgetItem = ui->listWidgetAutoResponse->currentItem();
    if ((listWidgetItem == nullptr) || (listWidgetItem == nullptr)){
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

void AutoResponseSettingPanel::clearAutoResponseItem()
{
    ui->listWidgetAutoResponse->clear();
    autoResponseItemList.clear();
}

void AutoResponseSettingPanel::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("xml (*.xml)"));
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)){
        qWarning() << file.errorString();
        return;
    }

    QDomDocument doc;

    /// 写入qml文件头
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    /// 添加根节点
    QDomElement root = doc.createElement(ITEM_ROOTNAME);
    doc.appendChild(root);

    QDomElement item;
    QDomElement itemChild;
    QDomText itemChildText;
    for (int index = 0; index < ui->listWidgetAutoResponse->count(); index++){
        item = doc.createElement("Item");
        itemChild = doc.createElement(ITEM_DESCRIPTION);
        itemChildText = doc.createTextNode(autoResponseItemList.at(index).description);
        itemChild.appendChild(itemChildText);
        item.appendChild(itemChild);

        itemChild   = doc.createElement(ITEM_RECEIVING_DATA);
        itemChildText  = doc.createTextNode(autoResponseItemList.at(index).receiveDataString);
        itemChild.appendChild(itemChildText);
        item.appendChild(itemChild);

        itemChild   = doc.createElement(ITEM_SENDING_DATA);
        itemChildText  = doc.createTextNode(autoResponseItemList.at(index).sendDataString);
        itemChild.appendChild(itemChildText);
        item.appendChild(itemChild);

        root.appendChild(item);
    }
    /// 保存为文件，缩进四空格
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

void AutoResponseSettingPanel::readIn()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save File"), "", tr("xml (*.xml)"));
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)){
        qWarning() << file.errorString();
        return;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.nodeName().compare(ITEM_ROOTNAME) != 0){
        QMessageBox::warning(this, tr("文件格式不相符"), tr("该文件的文件内容格不能被本软件读取，请检查后再试！"));
        return;
    }

    ui->listWidgetAutoResponse->clear();
    autoResponseItemList.clear();

    QDomNode node = root.firstChild();
    while(!node.isNull()) {
        if(node.isElement()){
            QDomElement e = node.toElement();
            QDomNodeList list = e.childNodes();
            addAutoResponseItem(list.at(1).toElement().text(), list.at(2).toElement().text(), list.at(0).toElement().text());
        }
        node = node.nextSibling();
    }
}

void AutoResponseSettingPanel::enableAutoResponseBtClicked()
{
    emit autoResponseFlagChanged(ui->checkBoxAutoResponse->isChecked());
}

void AutoResponseSettingPanel::setAutoResponseFlag(bool enableAutoResponse)
{
    ui->checkBoxAutoResponse->setChecked(enableAutoResponse);
}

bool AutoResponseSettingPanel::autoResponseFlag()
{
    return ui->checkBoxAutoResponse->isChecked();
}
