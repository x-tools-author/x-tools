/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    ,timingCheckBox (Q_NULLPTR)
    ,timingTimeLineEdit (Q_NULLPTR)
    ,textFormatComboBox (Q_NULLPTR)
    ,remarkLineEdit (Q_NULLPTR)
    ,inputDataTextEdit (Q_NULLPTR)
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

