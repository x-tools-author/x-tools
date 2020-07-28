/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QDateTime>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKOtherTimingSentItem.hh"

#include "ui_SAKOtherTimingSentItem.h"

SAKOtherTimingSentItem::SAKOtherTimingSentItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,ui(new Ui::SAKOtherTimingSentItem)
{
    initUi();
    id = QDateTime::currentMSecsSinceEpoch();
}

SAKOtherTimingSentItem::SAKOtherTimingSentItem(SAKDebugPage *debugPage,
                                               quint64 id,
                                               quint32 interval,
                                               quint32 format,
                                               QString comment,
                                               QString data,
                                               QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,id(id)
    ,ui(new Ui::SAKOtherTimingSentItem)
{
    initUi();

    timingTimeLineEdit->setText(QString::number(interval));
    textFormatComboBox->setCurrentIndex(format);
    descriptionLineEdit->setText(comment);
    inputDataTextEdit->setText(data);
}

SAKOtherTimingSentItem::~SAKOtherTimingSentItem()
{
    delete ui;
}

quint64 SAKOtherTimingSentItem::parameterID()
{
    return id;
}

quint32 SAKOtherTimingSentItem::parameterInterval()
{
    return timingTimeLineEdit->text().toUInt();
}

quint32 SAKOtherTimingSentItem::parameterFormat()
{
    return textFormatComboBox->currentIndex();
}

QString SAKOtherTimingSentItem::parameterComment()
{
    return descriptionLineEdit->text();
}

QString SAKOtherTimingSentItem::parameterData()
{
    return inputDataTextEdit->toPlainText();
}

void SAKOtherTimingSentItem::write()
{
    QString data = inputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->textFormatComboBox->currentData().toInt();
        debugPage->writeRawData(data, textFormat);
    }
}

void SAKOtherTimingSentItem::initUi()
{
    ui->setupUi(this);

    timingCheckBox = ui->timingCheckBox;
    timingTimeLineEdit = ui->timingTimeLineEdit;
    textFormatComboBox = ui->textFormatComboBox;
    descriptionLineEdit = ui->descriptionLineEdit;
    inputDataTextEdit = ui->inputDataTextEdit;
    updatePushButton = ui->updatePushButton;

    writeTimer.setInterval(timingTimeLineEdit->text().toInt());
    connect(&writeTimer, &QTimer::timeout, this, &SAKOtherTimingSentItem::write);

    SAKGlobal::initInputTextFormatComboBox(textFormatComboBox);
}

void SAKOtherTimingSentItem::on_timingCheckBox_clicked()
{
    if (timingCheckBox){
        timingCheckBox->isChecked() ? writeTimer.start() : writeTimer.stop();
    }
}

void SAKOtherTimingSentItem::on_timingTimeLineEdit_textChanged(const QString &text)
{
    int interval = text.toInt();
    writeTimer.setInterval(interval == 0 ? 1000 : interval);
}

void SAKOtherTimingSentItem::on_updatePushButton_clicked()
{
    QString tableName = SAKDataStruct::timingSendingTableName(debugPage->pageType());
    SAKDataStruct::SAKStructTimingSendingItem sendingItem;
    sendingItem.id = parameterID();
    sendingItem.data = parameterData();
    sendingItem.format = parameterFormat();
    sendingItem.comment = parameterComment();
    sendingItem.interval = parameterInterval();
//    SAKDebugPageDatabaseInterface *databaseInterface = SAKDebugPageDatabaseInterface::instance();
//    databaseInterface->updateTimingSendingItem(tableName, sendingItem);
}
