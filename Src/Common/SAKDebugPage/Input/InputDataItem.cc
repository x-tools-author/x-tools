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
#include "SAKBase.hh"
#include "SAKDebugPage.hh"
#include "InputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "InputDataFactory.hh"

#include "ui_InputDataItem.h"

InputDataItem::InputDataItem(SAKDebugPage *debugPage, DebugPageInputManager *inputManager, QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::InputDataItem)
    ,debugPage (debugPage)
    ,crcInterface (new SAKCRCInterface)
    ,factory (new InputDataFactory)
    ,inputManager (inputManager)
{
    ui->setupUi(this);
    factory->start();

    textFormatComboBox  = ui->textFormatComboBox;
    timingCheckBox      = ui->timingCheckBox;
    timingTimeLineEdit  = ui->timingTimeLineEdit;
    crcModelComboBox    = ui->crcModelComboBox;
    addCrcCheckBox      = ui->addCrcCheckBox;
    bigEndianCheckBox   = ui->bigEndianCheckBox;
    sendPushButton      = ui->sendPushButton;
    inputDataTextEdit   = ui->inputDataTextEdit;

    SAKBase::instance()->initTextFormatComboBox(textFormatComboBox);
    crcInterface->initCRCComboBox(crcModelComboBox);

    connect(&sendTimer, &QTimer::timeout, this, &InputDataItem::sendTimerTimeout);
    connect(this, &InputDataItem::rawDataChanged, factory, &InputDataFactory::cookData);
    connect(factory, &InputDataFactory::dataCooked, debugPage, &SAKDebugPage::write);
}

InputDataItem::~InputDataItem()
{
    delete ui;
    delete crcInterface;

    factory->terminate();
    delete factory;
}

void InputDataItem::on_textFormatComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    inputDataTextEdit->clear();
}

void InputDataItem::on_timingCheckBox_clicked()
{
    if (timingCheckBox->isChecked()){
        int interval = timingTimeLineEdit->text().toInt();
        if (interval == 0){
            interval = 1000;
        }
        sendTimer.start(interval);
    }else{
        sendTimer.stop();
    }
}

void InputDataItem::on_sendPushButton_clicked()
{
    sendTimerTimeout();
}

void InputDataItem::on_inputDataTextEdit_textChanged()
{
    inputManager->formattingInputText(inputDataTextEdit, textFormatComboBox->currentData().toInt());
}

void InputDataItem::sendTimerTimeout()
{
    inputParameters.addCRC = addCrcCheckBox->isChecked();
    inputParameters.crcModel = crcModelComboBox->currentData().toInt();
    inputParameters.bigEndian = bigEndianCheckBox->isChecked();
    inputParameters.inputModel = textFormatComboBox->currentData().toInt();

    QString data = inputDataTextEdit->toPlainText();
    if (!data.isEmpty()){
        emit rawDataChanged(data, inputParameters);
    }
}
