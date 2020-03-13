/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QDateTime>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKTimingSendingItemWidget.hh"

#include "ui_SAKTimingSendingItemWidget.h"

SAKTimingSendingItemWidget::SAKTimingSendingItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::SAKTimingSendingItemWidget)
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
    connect(&writeTimer, &QTimer::timeout, this, &SAKTimingSendingItemWidget::write);

    SAKGlobal::initInputTextFormatComboBox(textFormatComboBox);
}

SAKTimingSendingItemWidget::~SAKTimingSendingItemWidget()
{
    delete ui;
}

void SAKTimingSendingItemWidget::on_timingCheckBox_clicked()
{
    if (timingCheckBox){
        timingCheckBox->isChecked() ? writeTimer.start() : writeTimer.stop();
    }
}

void SAKTimingSendingItemWidget::on_timingTimeLineEdit_textChanged(const QString &text)
{
    int interval = text.toInt();
    writeTimer.setInterval(interval == 0 ? 1000 : interval);
}

void SAKTimingSendingItemWidget::write()
{
    QString data = inputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->textFormatComboBox->currentData().toInt();
        debugPage->writeRawData(data, textFormat);
    }
}

