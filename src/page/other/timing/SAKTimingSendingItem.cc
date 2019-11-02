/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKGlobal.hh"
#include "SAKTimingSendingItem.hh"

#include "ui_SAKTimingSendingItem.h"

#include <QDebug>
#include <QDateTime>

SAKTimingSendingItem::SAKTimingSendingItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::SAKTimingSendingItem)
    ,timingCheckBox (nullptr)
    ,timingTimeLineEdit (nullptr)
    ,textFormatComboBox (nullptr)
    ,remarkLineEdit (nullptr)
    ,inputDataTextEdit (nullptr)
    ,debugPage (debugPage)
{
    ui->setupUi(this);

    timingCheckBox      = ui->timingCheckBox;
    timingTimeLineEdit  = ui->timingTimeLineEdit;
    textFormatComboBox  = ui->textFormatComboBox;
    remarkLineEdit      = ui->remarkLineEdit;
    inputDataTextEdit   = ui->inputDataTextEdit;

    writeTimer.setInterval(timingTimeLineEdit->text().toInt());
    connect(&writeTimer, &QTimer::timeout, this, &SAKTimingSendingItem::write);

    SAKGlobal::initTextFormatComboBox(textFormatComboBox);
}

SAKTimingSendingItem::~SAKTimingSendingItem()
{
    delete ui;
}

void SAKTimingSendingItem::on_timingCheckBox_clicked()
{
    if (timingCheckBox){
        timingCheckBox->isChecked() ? writeTimer.start() : writeTimer.stop();
    }
}

void SAKTimingSendingItem::on_timingTimeLineEdit_textChanged(const QString &text)
{
    int interval = text.toInt();
    writeTimer.setInterval(interval == 0 ? 1000 : interval);
}

void SAKTimingSendingItem::write()
{
    QString data = inputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->textFormatComboBox->currentData().toInt();
        debugPage->writeRawData(data, textFormat);
    }
}

